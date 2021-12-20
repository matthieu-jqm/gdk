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

#ifndef __CONE_H__
#define __CONE_H__

#include <panda3d/geom.h>
#include <panda3d/geomTriangles.h>

#include "utils.h"


class Cone: public Geom {

    REGISTER_TYPE("Cone", Geom)

public:
    Cone(size_t levels=0);
    ~Cone() = default;

private:
    static GeomVertexData* get_geom_data(size_t levels);
    static GeomTriangles* subdivide(const GeomTriangles* prev);

    static PT(GeomVertexData) _vdata;
    static pvector<PT(GeomTriangles)> _primitives;
};
#endif // __CONE_H__