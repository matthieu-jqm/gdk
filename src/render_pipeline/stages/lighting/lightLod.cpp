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

#include <panda3d/instanceList.h>
#include <panda3d/graphicsStateGuardian.h>

#include "pointLightNode.h"
#include "spotLightNode.h"
#include "geodesic.h"
#include "cone.h"
#include "lightLod.h"

using namespace std;

template<> const int LightLod<LT_point>::_segments = 6;
template<> const int LightLod<LT_spot>::_segments = 4;


template<>
const Geom* LightLod<LT_point>::make_geom(int n) {
    return new Geodesic(n);
}

template<>
const Geom* LightLod<LT_spot>::make_geom(int n) {
    return new Cone(n);
}


template<>
void LightLod<LT_point>::add_instance(GraphicsStateGuardian* gsg,
                    PandaNode* node, const TransformState* transform)
{

    PointLightNode* light = DCAST(PointLightNode, node);

    float radius = light->get_radius();
    
    LMatrix4 mat = transform->get_mat();
    LPoint3 pos = mat.get_row3(3);;

    float distance = gsg->compute_distance_to(pos);
    float scale = radius * _scale_bias;

    DataType data;
    data.color = light->get_color() * light->get_energy();
    data.shadow_id = light->get_shadow_slot();
    data.min_depth = distance - radius;
    data.max_depth = distance + radius;
    data.radius = radius;
    data.position = pos;

    _buffer.push_back(data);
    _instances->append(transform->set_scale(scale));
}


template<>
void LightLod<LT_spot>::add_instance(GraphicsStateGuardian* gsg,
                    PandaNode* node, const TransformState* transform)
{

    SpotLightNode* light = DCAST(SpotLightNode, node);

    float max_distance = light->get_max_distance();
    float outter_radius = light->get_outter_radius();

    LMatrix4 mat = transform->get_mat();
    LPoint3 pos = mat.get_row3(3);
    LVector3 dir = mat.get_row3(2);

    PN_stdfloat pos_d = gsg->compute_distance_to(pos);
    PN_stdfloat dir_d = gsg->compute_distance_to(dir);
    
    float b = pos_d + dir_d * max_distance;
    float r = length(dir.get_xy()) * outter_radius;

    auto minmax_d = std::minmax({pos_d, b + r, b - r});
    float sxy = outter_radius * _scale_bias;

    LVector3 scale(sxy, sxy, max_distance);

    DataType data;
    data.color = light->get_color() * light->get_energy();
    data.shadow_id = light->get_shadow_slot();
    data.min_depth = minmax_d.first;
    data.max_depth = minmax_d.second;
    data.direction = dir;
    data.position = pos;
    data.max_distance = max_distance;
    data.outter_radius = outter_radius;
    data.inner_radius = light->get_inner_radius();

    _buffer.push_back(data);
    _instances->append(transform->set_scale(scale));
}

