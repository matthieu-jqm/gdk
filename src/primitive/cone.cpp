#include "cone.h"
#include <panda3d/geomVertexWriter.h>
#include <panda3d/geomTriangles.h>
#include <panda3d/geomVertexRewriter.h>
#include <complex>

using namespace std;

DEFINE_TYPEHANDLE(Cone)

PT(GeomVertexData) Cone::_vdata;
pvector<PT(GeomTriangles)> Cone::_primitives;


Cone::Cone(size_t levels): Geom(get_geom_data(levels)) {

    add_primitive(_primitives[levels]);
}


GeomVertexData* Cone::get_geom_data(size_t level) {

    if (_primitives.empty()) {

        _vdata = new GeomVertexData("cone", 
                                GeomVertexFormat::get_v3(), 
                                GeomEnums::UH_static);
    
        GeomVertexWriter vertices(_vdata, InternalName::get_vertex());
        
        GeomTriangles* primitive = new GeomTriangles(GeomEnums::UH_static);
        _primitives.push_back(primitive);


        static LVecBase3f vdata[6] = {
            {0.0,  0.0,  0.0}, { 0.0, 0.0, -1.0}, {1.0, 0.0, -1.0},
            {0.0, -1.0, -1.0}, {-1.0, 0.0, -1.0}, {0.0, 1.0, -1.0}
        };

        static int tdata[8][3] = {
            {0,3,2}, {1,2,3}, {0,4,3}, {1,3,4}, 
            {0,5,4}, {1,4,5}, {0,2,5}, {1,5,2}
        };

        for (int i = 0; i < 6; ++i)
            vertices.add_data3f(vdata[i]);

        for (int i = 0; i < 8; ++i)
            primitive->add_vertices(tdata[i][0], tdata[i][1], tdata[i][2]);
    };

    while(_primitives.size() <= level)
        _primitives.push_back(subdivide(_primitives.back()));

    return _vdata;
}


GeomTriangles* Cone::subdivide(const GeomTriangles* prev) {

    GeomTriangles* primitive = new GeomTriangles(GeomEnums::UH_static);
    GeomVertexRewriter vertices(_vdata, InternalName::get_vertex());


    int num_indices = prev->get_num_vertices();
    int num_segment = num_indices / 6;

    vertices.set_row(3);

    LVecBase3f v3 = vertices.get_data3f();
    LVecBase2f v = normalize(v3.get_xy() + LVecBase2f(1.0f, 0.0f));

    complex<float> current_vect(v.get_x(), v.get_y());
    complex<float> rotation_vect(v3.get_x(), v3.get_y());

    int num_vertex = _vdata->get_num_rows();
    vertices.set_row(num_vertex);


    for (int i=0; i<num_segment; ++i) {
        vertices.add_data3f(current_vect.real(), current_vect.imag(), -1.0f);
        current_vect *= rotation_vect;
    }


    int nv = num_vertex;

    for (int i=0; i<num_indices; i+=6) {

        int s0 = prev->get_vertex(i+0);
        int s1 = prev->get_vertex(i+1);
        int s2 = prev->get_vertex(i+2);

        int b0 = prev->get_vertex(i+3);
        int b1 = prev->get_vertex(i+4);
        int b2 = prev->get_vertex(i+5);

        primitive->add_vertices(s0, s1, nv);
        primitive->add_vertices(b0, b1, nv);
        primitive->add_vertices(s0, nv, s2);
        primitive->add_vertices(b0, nv, b2);

        ++nv;
    }

    return primitive;
}


    // GeomVertexData* data = new GeomVertexData("icosahedron", 
    //                                         GeomVertexFormat::get_v3n3(), 
    //                                         GeomEnums::UH_static);

    // GeomVertexWriter vertices(data);

    // GeomTriangles* primitive = new GeomTriangles(GeomEnums::UH_static);

    // vertices.add_data3f(0.0f, 0.0f, 0.0f); // top vertex
    // vertices.add_data3f(0.0f, 0.0f,-1.0f); // bottom vertex

    // float angle = 0.0f;
    // float segment_angle = M_PI*2 / segment;

    // for (int i = 0; i < segment; ++i) {
    //     vertices.add_data3f(cosf(angle), sinf(angle), -1.0f);
    //     int vert = i + 2;
    //     int next_vert = (i+1)%segment + 2;
    //     primitive->add_vertices(0, vert, next_vert); // side triangle
    //     primitive->add_vertices(1, vert, next_vert); // bottom triangle
    //     angle += segment_angle;
    // }

    // primitive->close_primitive();

    // Geom* geom = new Geom(data);
    // add_geom(geom);
    // geom->add_primitive(primitive);