#include "geodesic.h"
#include <panda3d/geomVertexRewriter.h>
#include <panda3d/geomTriangles.h>
#include <map>

using namespace std;


DEFINE_TYPEHANDLE(Geodesic)

pvector<PT(GeomTriangles)> Geodesic::_primitives;
PT(GeomVertexData) Geodesic::_vdata;


Geodesic::Geodesic(size_t levels): Geom(get_geom_data(levels)) {

    add_primitive(_primitives[levels]);
}


GeomVertexData* Geodesic::get_geom_data(size_t levels) {

    if (_primitives.empty()) {
    
        _vdata = new GeomVertexData("geodesic", 
                                GeomVertexFormat::get_v3(), 
                                GeomEnums::UH_static);

        GeomVertexWriter vertices(_vdata, InternalName::get_vertex());

        GeomTriangles* primitive = new GeomTriangles(GeomEnums::UH_static);
        _primitives.push_back(primitive);

        static const float X = .525731112119133606;
        static const float Z = .850650808352039932;

        static LVecBase3f vdata[12] = {
            {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
            {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
            {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
        };

        static int tdata[20][3] = {
            {4,0,1}, {9,0,4}, {5,9,4}, {5,4,8}, {8,4,1},
            {10,8,1}, {3,8,10}, {3,5,8}, {2,5,3}, {7,2,3},
            {10,7,3}, {6,7,10}, {11,7,6}, {0,11,6}, {1,0,6},
            {1,6,10}, {0,9,11}, {11,9,2}, {2,9,5}, {2,7,11}
        };

        for (int i = 0; i < 12; ++i)
            vertices.add_data3f(vdata[i]);

        for (int i = 0; i < 20; ++i)
            primitive->add_vertices(tdata[i][0], tdata[i][1], tdata[i][2]);
    };

    while(_primitives.size() <= levels)
        _primitives.push_back(subdivide(_primitives.back()));

    return _vdata;
}


GeomTriangles* Geodesic::subdivide(const GeomTriangles* prev) {

    GeomTriangles* primitive = new GeomTriangles(GeomEnums::UH_static);
    GeomVertexRewriter vertices(_vdata, InternalName::get_vertex());

    typedef unsigned long long Key;
    map<Key,int> edges;

    int end = prev->get_num_vertices();

    for (int i=0; i<end; i+=3) {
        
        int ids0[3],  // indices of outer vertices
            ids1[3];  // indices of edge vertices

        for (int j=0; j<3; ++j) {

            int j1 = (j + 1) % 3;
            int e0 = prev->get_vertex(i+j);
            int e1 = prev->get_vertex(i+j1);
            ids0[j] = e0;

            if (e1 > e0)
                swap(e0,e1);

            Key key = Key(e0) | (Key(e1) << 32);
            map<Key,int>::iterator it = edges.find(key);

            if (it == edges.end()) {

                int write_row = _vdata->get_num_rows();
                edges[key] = write_row;
                ids1[j] = write_row;
                
                vertices.set_row(e0);
                LVecBase3 v0 = vertices.get_data3f();

                vertices.set_row(e1);
                LVecBase3 v1 = vertices.get_data3f();
                
                vertices.set_row(write_row);
                vertices.add_data3f(normalize(v0 + v1));
            }
            else
                ids1[j] = it->second;
        }

        primitive->add_vertices(ids0[0], ids1[0], ids1[2]);
        primitive->add_vertices(ids0[1], ids1[1], ids1[0]);
        primitive->add_vertices(ids0[2], ids1[2], ids1[1]);
        primitive->add_vertices(ids1[0], ids1[1], ids1[2]);
    }

    return primitive;
}

// Geodesic::Geodesic(const string& name, int num_subdive): GeomNode(name) {

//     if (!_base_init)
//         generate_icosahedron();

    


//     // vector<float> tmpVertices;
//     // vector<float> tmpIndices;
//     // const float *v1, *v2, *v3;          // ptr to original vertices of a triangle
//     // float newV1[3], newV2[3], newV3[3]; // new vertex positions
//     // unsigned int index;

//     LVecBase3f vert0, vert1, vert2;

//     for(int i = 1; i <= num_subdive; ++i) {

//         tmpVertices = vertices;
//         tmpIndices = indices;
//         vertices.clear();
//         indices.clear();
//         index = 0;

//         // perform subdivision for each triangle
//         for(int j = 0; j < tmpIndices.size(); j += 3) {

//             // get 3 vertices of a triangle
//             v0 = &tmpVertices[tmpIndices[j]];
//             v1 = &tmpVertices[tmpIndices[j + 1]];
//             v2 = &tmpVertices[tmpIndices[j + 2]];

//             // compute 3 new vertices by spliting half on each edge
//             //         v1       
//             //        / \       
//             // newV1 *---* newV3
//             //      / \ / \     
//             //    v2---*---v3   
//             //       newV2      
//             vert0 = normalize(v0 + v1);
//             vert1 = normalize(v1 + v2);
//             vert2 = normalize(v2 + v0);

//             // add 4 new triangles to vertex array
//             addVertices(v1,    newV1, newV3);
//             addVertices(newV1, v2,    newV2);
//             addVertices(newV1, newV2, newV3);
//             addVertices(newV3, newV2, v3);

//             // add indices of 4 new triangles
//             addIndices(index,   index+1, index+2);
//             addIndices(index+3, index+4, index+5);
//             addIndices(index+6, index+7, index+8);
//             addIndices(index+9, index+10,index+11);
//             index += 12;    // next index
//         }
//     }
// }



// void Geodesic::generate_icosahedron() {
    
//     static float H_ANGLE = deg_2_rad(72.0);
//     static float V_ANGLE = atanf(1.0f / 2);

//     float z, xy;
//     float angle_0 = -M_PI / 2 - H_ANGLE / 2;
//     float angle_1 = -M_PI / 2;

//     z  = sinf(V_ANGLE);
//     xy = cosf(H_ANGLE);

//     _base[0] = LVecBase3f(0,0,1);

//     for(int i = 1; i <= 5; ++i) {

//         _base[i] = LVecBase3f(xy*cosf(angle_0), xy*sinf(angle_0), z);
//         _base[i + 5] = LVecBase3f(xy*cosf(angle_1), xy*sinf(angle_0), -z);

//         angle_0 += H_ANGLE;
//         angle_1 += H_ANGLE;
//     }

//     _base[11] = LVecBase3f(0,0,-1); 
//     _base_init = true;
// }