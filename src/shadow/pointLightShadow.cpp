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

#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/look_at.h>

#include "render_utils.h"
#include "shadowBufferManager.h"
#include "pointLightNode.h"
#include "pointLightShadow.h"

using namespace std;

DEFINE_TYPEHANDLE(PointLightShadow)

PointLightShadow::Faces PointLightShadow::_faces;


void PointLightShadow::post_init() {

    look_at(_faces[0], LVector3( 0.0f, 0.0f,-1.0f), LVector3(-1.0f, 0.0f, 0.0f));
    look_at(_faces[1], LVector3( 0.0f, 0.0f,-1.0f), LVector3( 1.0f, 0.0f, 0.0f));

    look_at(_faces[2], LVector3( 0.0f, 1.0f, 0.0f), LVector3(0.0f, 0.0f, 1.0f));
    look_at(_faces[3], LVector3( 0.0f,-1.0f, 0.0f), LVector3(0.0f, 0.0f,-1.0f));

    look_at(_faces[4], LVector3( 0.0f, 0.0f,-1.0f), LVector3(0.0f, 1.0f, 0.0f));
    look_at(_faces[5], LVector3( 0.0f, 0.0f,-1.0f), LVector3(0.0f,-1.0f, 0.0f));
}


void PointLightShadow::cull(DisplayRegionCullCallbackData* data) {

    SceneSetup* setup = data->get_scene_setup();
    const Lens* lens = setup->get_lens();

    LMatrix4 proj = lens->get_projection_mat();

    for (int i=0; i<6; ++i)
        _projections[i] = _faces[i] * proj;

    ShadowSlot::cull(data);
}


ShadowSlot* PointLightShadow::make(ShadowBuffer* buffer) {
    return new PointLightShadow(buffer);
}


PointLightShadow::PointLightShadow(ShadowBuffer* buffer):
    MultiViewShadowSlot(buffer)
{
    
}

ShadowBufferInfo PointLightShadow::get_info() {

    ShadowBufferInfo info;
    info.name = "point";
    info.light_type = PointLightNode::get_class_type();
    info.buffer_type = Texture::TT_cube_map_array;
    info.flags = GraphicsPipe::BF_size_square;
    info.views = 6;
    info.make_slot = make;

    return info;
}
