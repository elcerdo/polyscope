#include "polyscope/polyscope.h"

#include "geometrycentral/geometry.h"
#include "geometrycentral/halfedge_mesh.h"
#include "geometrycentral/polygon_soup_mesh.h"
#include "geometrycentral/direction_fields.h"

#include <memory>
#include <random>

void buildPyramid() {
    std::default_random_engine rng;

    std::vector<Vector3> positions;
    std::vector<std::vector<size_t>> polys;
    {
        const size_t nn = 60;
        const size_t mm = 45;

        positions.reserve(nn * mm);
        for (size_t ll=0; ll<mm; ll++)
            for (size_t kk=0; kk<nn; kk++)
            {
                const double theta = 2*M_PI*kk/static_cast<double>(nn);
                const double zz = 2*ll/static_cast<double>(mm) - 1;
                positions.emplace_back(Vector3{cos(theta), sin(theta), zz});
            }

        std::bernoulli_distribution dist(.1);

        polys.reserve((mm-1)*nn);
        for (size_t ll=0; ll<mm-1; ll++)
            for (size_t kk=0; kk<nn; kk++)
            {
                const size_t aa = ll*nn+kk;
                const size_t bb = (ll+1)*nn+kk;
                const size_t cc = (ll+1)*nn+(kk+1)%nn;
                const size_t dd = ll*nn+(kk+1)%nn;
                polys.emplace_back((ll + kk) % 2 == dist(rng) ? std::vector<size_t>{aa, bb, cc, dd} : std::vector<size_t>{bb, cc, dd, aa});
            }
    }

    PolygonSoupMesh soup(polys, positions);
    soup.triangulate();

    Geometry<Euclidean>* geom = nullptr;
    HalfedgeMesh* mesh = new HalfedgeMesh(soup, geom);

    polyscope::registerSurfaceMesh("pyramid", geom);

    {
        polyscope::SurfaceMesh* surface = polyscope::getSurfaceMesh("pyramid");
        std::normal_distribution<double> dist();
        VertexData<double> vertex_scalars(mesh);
        for (const VertexPtr& vertex : mesh->vertices())
            vertex_scalars[vertex] = geom->position(vertex).z;

        surface->addQuantity("vertex_scalar", vertex_scalars);

        VertexData<Complex> vertex_direction = computeSmoothestVertexDirectionField(surface->originalGeometry);
        surface->addVectorQuantity("vertex_direction", vertex_direction);

        FaceData<Complex> face_direction = computeSmoothestFaceDirectionField(surface->originalGeometry);
        surface->addVectorQuantity("face_direction", face_direction);

        delete geom;
        delete mesh;
    }
}

void solveSmoothField() {
}

int main(int argc, char** argv) {
    polyscope::options::autocenterStructures = true;

    polyscope::init();

    buildPyramid();
    solveSmoothField();

    polyscope::show();

    return 0;
}
