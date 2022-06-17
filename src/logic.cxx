#include "lib3mf_implicit.hpp"
#include <iostream>
#include <meh/shims.hpp>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/SurfaceTriangulation.h>

using namespace Lib3MF;
using namespace pmp;

namespace meh {
bool exportModelListTo3MF(char *in[], char *inTex[], char *out, int models) {
  // Create wrapper object for 3mf
  PWrapper wrapper = wrapper->loadLibrary();
  PModel model = wrapper->CreateModel();

  // Create base mesh object to access file data
  SurfaceMesh mesh;

  // Check 3MF version
  Lib3MF_uint32 nMajor, nMinor, nMicro;
  wrapper->GetLibraryVersion(nMajor, nMinor, nMicro);

  std::cout << "Starting 3MF generation version:" << nMajor << "." << nMinor
            << "." << nMicro << std::endl;

  for (int i = 0; i < models; i++) {
    std::cout << "Processing " << in[i] << std::endl;

    // Load mesh file
    mesh.read(in[i]);

    // ensure mesh has only tris, else skip it.
    std::cout << "Checking mesh triangulation" << std::endl;
    if (!mesh.is_triangle_mesh()) {
      std::cout << "Mesh not triangulated, triangulating manually" << std::endl;
      std::cout << "WARNING: This will destroy UV's if this mesh has UV data, "
                   "this technique will result in unpredictable results"
                << std::endl;
      SurfaceTriangulation divider(mesh);
      divider.triangulate();
    }

    // Create 3MF mesh object
    std::cout << "Createing new 3mf object" << std::endl;
    PMeshObject mesh3MF = model->AddMeshObject();
    std::string modelPath = std::string(in[i]);
    std::string modelName =
        modelPath.substr(modelPath.rfind("/"), modelPath.rfind("."));
    mesh3MF->SetName(modelName);

    // Create mesh elements
    std::vector<sLib3MFPosition> vertices(mesh.n_vertices());
    std::vector<sLib3MFTriangle> triangles(mesh.n_faces());

    // Check for color elements
    std::cout << "Checking for vertex color" << std::endl;
    bool hasVertexColor = mesh.has_vertex_property("v:color");
    PColorGroup colorgroup;
    VertexProperty<Color> colors;
    if (hasVertexColor) {
      std::cout << "Vertex color found, initializing." << std::endl;
      colors = mesh.get_vertex_property<Color>("v:color");
      colorgroup = model->AddColorGroup();
    }

    // Add points to 3MF mesh
    std::cout << "Copying points over" << std::endl;
    auto points = mesh.get_vertex_property<Point>("v:point");
    for (auto v : mesh.vertices()) {
      vertices[v.idx()] = convertPMPto3MFVertex(points[v]);
      if (hasVertexColor) {
        colorgroup->AddColor(convertPMPTo3MFVertexColor(colors[v]));
      }
    }

    // Add tris to 3MF mesh
    std::cout << "Copying tris over" << std::endl;
    for (auto f : mesh.faces()) {
      SurfaceMesh::VertexAroundFaceCirculator vafc = mesh.vertices(f);
      triangles[f.idx()] = convertPMPto3MFFace(vafc);
    }

    // Set the 3MF mesh to contain the vertex and tris previously created
    mesh3MF->SetGeometry(vertices, triangles);

    // Add vertex color to 3MF wrapper, if it existed in the object
    std::cout << "Adding vertex color to 3mf" << std::endl;
    if (hasVertexColor) {
      for (auto f : mesh.faces()) {
        SurfaceMesh::VertexAroundFaceCirculator vafc = mesh.vertices(f);
        mesh3MF->SetTriangleProperties(
            f.idx(), convertPMPTo3MFTriangleColorProperties(vafc, colorgroup));
      }

      Lib3MF_uint32 idW =
          colorgroup->AddColor(wrapper->RGBAToColor(255, 255, 255, 0));
      mesh3MF->SetObjectLevelProperty(colorgroup->GetResourceID(), idW);
    }

    // Check for texture elements
    std::cout << "Looking for UVs" << std::endl;
    bool hasUVs = mesh.has_halfedge_property("h:tex");
    if (hasUVs) {
      std::cout << "UVs found, populating info" << std::endl;
      std::string filePath = std::string(inTex[i]);
      std::string fileName =
          filePath.substr(filePath.rfind("/"), filePath.length());
      std::string fileType =
          filePath.substr(filePath.rfind("."), filePath.length());
      std::string path3mf = "/3D/Textures" + modelName.substr(0, modelName.rfind(".")) + fileType;
      std::string pathFile = inTex[i];
      std::string sRelationshipType_Texture =
          "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";

      // Try to add new texture. If it already exists, use existing texture
      // instead
      PAttachment attachment =
          model->AddAttachment(path3mf, sRelationshipType_Texture);
      attachment->ReadFromFile(pathFile);

      PTexture2D texture2D =
          model->AddTexture2DFromAttachment(attachment.get());

      if (fileType == ".png") {
        texture2D->SetContentType(eTextureType::PNG);
      } else if (fileType == ".jpg" || fileType == ".jpeg") {
        texture2D->SetContentType(eTextureType::JPEG);
      } else {
        texture2D->SetContentType(eTextureType::Unknown);
      }

      texture2D->SetTileStyleUV(eTextureTileStyle::Wrap,
                                eTextureTileStyle::Wrap);

      PTexture2DGroup textureGroup = model->AddTexture2DGroup(texture2D.get());

      auto uvs = mesh.get_halfedge_property<TexCoord>("h:tex");
      for (auto f : mesh.faces()) {
        SurfaceMesh::HalfedgeAroundFaceCirculator hafc = mesh.halfedges(f);

        mesh3MF->SetTriangleProperties(
            f.idx(), convertPMPTo3MFUVProperties(hafc, uvs, textureGroup));
      }

      mesh3MF->SetObjectLevelProperty(textureGroup->GetResourceID(), 1);
    }

    // Add model to build list
    model->AddBuildItem(mesh3MF.get(), wrapper->GetIdentityTransform());
  }

  // Wrtie to file
  PWriter writer = model->QueryWriter("3mf");
  writer->WriteToFile(out);

  std::cout << "Model exported.";
  return true;
}
} // namespace meh
