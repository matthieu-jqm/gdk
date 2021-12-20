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

#ifndef __LIGHTDATA_H__
#define __LIGHTDATA_H__

#include <panda3d/aa_luse.h>

#include "render_utils.h"
#include "utils.h"

#define REGISTER_LIGHT_TYPE(type) \
    _REGISTER_TEMPLATE_NAME(LT_ ## type, #type)

#define LIGHT_DATA(type) \
    template<> \
    struct alignas(16) LightData<type>: public LightDataBase<type>


#define BIND_LIGHT_TYPE(type, cls) \
    template<> TypeHandle LightDataBase<type>::get_class_type() { \
        return cls::get_class_type(); \
    }


enum LightType {
    LT_point = 0,
    LT_spot,
    LT_area,
    LT_tube,
    LT_directional,
    LT_COUNT
};


std::string get_light_name(LightType type);


template<LightType Type>
class LightData {};


template<LightType Type>
struct LightDataBase {

    static TypeHandle get_class_type();

    glsl::vec3 color;
    uint32_t shadow_id;
};

struct ShadowData {
    LMatrix4 transform;
    LVector4 data;
};


LIGHT_DATA(LT_point) {

    float min_depth;
    float max_depth;
    float radius;

    alignas(16) glsl::vec3 position;
};


LIGHT_DATA(LT_spot) {

    float min_depth;
    float max_depth;
    float outter_radius;
    float inner_radius;
    float max_distance;

    alignas(16) glsl::vec3 position;
    alignas(16) glsl::vec3 direction;
};


LIGHT_DATA(LT_directional) {

    alignas(16) glsl::vec3 direction;
    float angular_diameter;
};

#endif // __LIGHTDATA_H__