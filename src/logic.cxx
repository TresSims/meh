#include "lib3mf_implicit.hpp"
#include <iostream>
#include <meh/shims.hpp>
#include <pmp/SurfaceMesh.h>

using namespace Lib3MF;
using namespace pmp;

namespace meh {
bool exportModelListTo3MF(char *in[], char *out, int models) {
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
    if (!mesh.is_triangle_mesh()) {
      std::cout << "Mesh has non-tri elements, stopping." << std::endl;
    }

    // Create 3MF mesh object
    PMeshObject mesh3MF = model->AddMeshObject();
    mesh3MF->SetName(in[i]);

    // Create mesh elements
    std::vector<sLib3MFPosition> vertices(mesh.n_vertices());
    std::vector<sLib3MFTriangle> triangles(mesh.n_faces());

    // Check for color elements
    bool hasVertexColor = mesh.has_vertex_property("v:color");
    PColorGroup colorgroup;
    VertexProperty<Color> colors;
    if (hasVertexColor) {
      colors = mesh.get_vertex_property<Color>("v:color");
      colorgroup = model->AddColorGroup();
    }

    // Add points to 3MF mesh
    auto points = mesh.get_vertex_property<Point>("v:point");
    for (auto v : mesh.vertices()) {
      vertices[v.idx()] = convertPMPto3MFVertex(points[v]);
      if (hasVertexColor) {
        colorgroup->AddColor(convertPMPTo3MFVertexColor(colors[v]));
      }
    }

    // Add tris to 3MF mesh
    for (auto f : mesh.faces()) {
      SurfaceMesh::VertexAroundFaceCirculator vafc = mesh.vertices(f);
      triangles[f.idx()] = convertPMPto3MFFace(vafc);
    }

    // Set the 3MF mesh to contain the vertex and tris previously created
    mesh3MF->SetGeometry(vertices, triangles);

    // Add vertex color to 3MF wrapper, if it existed in the object
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
