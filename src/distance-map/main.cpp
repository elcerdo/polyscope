#include "polyscope/polyscope.h"

#include "geometrycentral/geometry.h"
#include "geometrycentral/halfedge_mesh.h"
#include "geometrycentral/polygon_soup_mesh.h"

void buildPyramid() {
    const std::vector<Vector3> positions = {
        {-1,-1, 0},
        {1, -1, 0},
        {1, 1, 0},
        {-1, 1, 0},
        {0, 0, 1},
    };

    const std::vector<std::vector<std::size_t>> polys = {
        {3, 2, 1, 0},
        {0, 1, 4},
        {1, 2, 4},
        {2, 3, 4},
        {3, 0, 4},
    };

    PolygonSoupMesh soup(polys, positions);
    soup.triangulate();

    Geometry<Euclidean>* geom = nullptr;
    HalfedgeMesh* mesh = new HalfedgeMesh(soup, geom);

    polyscope::registerSurfaceMesh("pyramid", geom);

    VertexData<double> vertex_scalars(mesh);
    for (const VertexPtr& vertex : mesh->vertices())
        vertex_scalars[vertex] = geom->position(vertex).z;

    polyscope::getSurfaceMesh("pyramid")->addQuantity("vertex_scalar", vertex_scalars);

    delete geom;
    delete mesh;
}

int main(int argc, char** argv) {
    polyscope::options::autocenterStructures = true;

    polyscope::init();

    buildPyramid();

    polyscope::show();

    return 0;
}
