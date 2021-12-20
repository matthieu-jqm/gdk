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

#ifndef __LIGHTLOD_H__
#define __LIGHTLOD_H__


#include "utils.h"
#include "lightData.h"


class Geom;
class InstanceList;
class TransformState;


template<LightType T>
class LightLod {

public:
    using DataType = LightData<T>;

    LightLod(int n);
    ~LightLod() = default;

    void reset();
    void collect_data(uchar*&) const;

    void add_instance(GraphicsStateGuardian* gsg,
            PandaNode* node, const TransformState* transform);

    const Geom* get_geom() const;
    InstanceList* get_instances() const;
    size_t get_num_instances() const;

private:
    static const Geom* make_geom(int n);
    static const int _segments;

    typedef std::vector<DataType> DataBuffer;

    PT(InstanceList) _instances;
    CPT(Geom) _geom;

    float _scale_bias;
    DataBuffer _buffer;
};


#include "lightLod.T"

#endif // __LIGHTLOD_H__