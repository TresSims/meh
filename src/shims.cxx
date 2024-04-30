#include "Cpp/lib3mf_implicit.hpp"
#include <pmp/surface_mesh.h>

using namespace Lib3MF;
using namespace pmp;

namespace meh {
sLib3MFPosition convertPMPto3MFVertex(Point p) {
  /*
   * Converts a pmp-library Point object to a lib3MF sLib3MFPosition.
   */

  sLib3MFPosition result;
  result.m_Coordinates[0] = p[0];
  result.m_Coordinates[1] = p[1];
  result.m_Coordinates[2] = p[2];
  return result;
}

sLib3MFTriangle convertPMPto3MFFace(SurfaceMesh::VertexAroundFaceCirculator f) {
  /*
   * Takes a pmp-library VertexAroundFaceCirculator and puts the
   * indicies of the circulated vertex's into a lib3MF
   * sLib3MFTriangle object.
   */

  sLib3MFTriangle result;
  int i = 2;
  for (auto v : f) {
    result.m_Indices[i] = v.idx();
    i--;
  }
  return result;
}

sColor convertPMPTo3MFVertexColor(Color color) {
  /*
   * Takes a pmp-library Color and converts it to a lib3MF sColor
   */

  sColor result;
  result.m_Red = color[0];
  result.m_Green = color[1];
  result.m_Blue = color[2];
  result.m_Alpha = 255;
  return result;
}

sTriangleProperties convertPMPTo3MFTriangleColorProperties(
    SurfaceMesh::VertexAroundFaceCirculator f, PColorGroup resource) {
  /*
   * \brief Creates a lib3MF sTriangleProperty from a list of vertices,
   *        and a color resource.
   * Currenlty this implementation assumes your color resource is indexed
   * the same as your vertices, this could be changed in the future to
   * allow for multiple vertices with the same color point to the same
   * resource index, rather than having two identical resources.
   */

  sTriangleProperties result;
  result.m_ResourceID = resource->GetResourceID();
  int i = 2;
  for (auto v : f) {
    result.m_PropertyIDs[i] = v.idx() + 1;
    i--;
  }
  return result;
}

sTriangleProperties
convertPMPTo3MFUVProperties(SurfaceMesh::HalfedgeAroundFaceCirculator f,
                            HalfedgeProperty<TexCoord> uvList,
                            PTexture2DGroup resource) {
  /*
   * \brief Creates a lib3MF sTriangleProperty from a list of vertices,
   *        and a color resource.
   * Currenlty this implementation assumes your color resource is indexed
   * the same as your vertices, this could be changed in the future to
   * allow for multiple vertices with the same color point to the same
   * resource index, rather than having two identical resources.
   */

  sTriangleProperties result;
  result.m_ResourceID = resource->GetResourceID();
  int i = 2;
  for (auto v : f) {
    const TexCoord &UVs = uvList[v];
    result.m_PropertyIDs[i] =
        resource->AddTex2Coord(sLib3MFTex2Coord({UVs[0], UVs[1]}));
    i--;
  }
  return result;
}
} // namespace meh
