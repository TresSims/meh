#pragma once

#include "lib3mf_implicit.hpp"
#include <pmp/SurfaceMesh.h>

using namespace Lib3MF;
using namespace pmp;

namespace meh {
sLib3MFPosition convertPMPto3MFVertex(Point p);

sLib3MFTriangle convertPMPto3MFFace(SurfaceMesh::VertexAroundFaceCirculator f);

sColor convertPMPTo3MFVertexColor(Color color);

sTriangleProperties convertPMPTo3MFTriangleColorProperties(
    SurfaceMesh::VertexAroundFaceCirculator f, PColorGroup resource);

sTriangleProperties
convertPMPTo3MFUVProperties(SurfaceMesh::HalfedgeAroundFaceCirculator f,
                            HalfedgeProperty<TexCoord> uvList,
                            PTexture2DGroup resource);
} // namespace meh
