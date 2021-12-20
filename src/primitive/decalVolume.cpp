// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// This file is part of Among Z.
// 
// Among Z is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Among Z is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Among Z.  If not, see <http://www.gnu.org/licenses/>.


#include "decalVolume.h"
#include <panda3d/geomTriangles.h>
#include <panda3d/geomVertexWriter.h>

using namespace std;


DEFINE_TYPEHANDLE(DecalVolume)

PT(GeomVertexData) DecalVolume::_vertices;
PT(GeomTriangles) DecalVolume::_primitive = make_primitive();


DecalVolume::DecalVolume(): Geom(make_vertex_data()) {
    
    add_primitive(make_primitive());
}


PT(GeomTriangles) DecalVolume::make_primitive() {

    if (UNLIKELY(_primitive == nullptr)) {
        _primitive = new GeomTriangles(GeomEnums::UH_static);

        static int indices[12][3] = {
            {1,0,3}, {2,1,3}, {6,1,2}, {5,1,6}, 
            {6,2,3}, {6,3,7}, {6,7,5}, {4,5,7},
            {0,5,4}, {1,5,0}, {0,7,3}, {4,7,0}
        };

        for (int i = 0; i < 12; ++i)
            _primitive->add_vertices(indices[i][0], indices[i][1], indices[i][2]);
    }
    return _primitive;
}


PT(GeomVertexData) DecalVolume::make_vertex_data() {

    if (UNLIKELY(_vertices == nullptr)) {
        _vertices = new GeomVertexData("decal_volume", 
                                GeomVertexFormat::get_v3(), 
                                GeomEnums::UH_static);

        GeomVertexWriter writer(_vertices, InternalName::get_vertex());

        static const LVecBase3f vertices[8] = {
            {-1.0, -1.0,  0.0}, {-1.0,  1.0,  0.0}, 
            { 1.0,  1.0,  0.0}, { 1.0, -1.0,  0.0},
            {-1.0, -1.0, -1.0}, {-1.0,  1.0, -1.0}, 
            { 1.0,  1.0, -1.0}, { 1.0, -1.0, -1.0}
        };

        for (const LVecBase3f& vertex : vertices)
            writer.add_data3f(vertex);
    }
    return _vertices;
}

