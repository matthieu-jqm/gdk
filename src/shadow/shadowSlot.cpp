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

#include <panda3d/graphicsEngine.h>
#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/displayRegionDrawCallbackData.h>

#include "shadowBuffer.h"
#include "lightBase.h"
#include "shadowSlot.h"


using namespace std;


DEFINE_TYPEHANDLE(ShadowSlot)



void ShadowSlot::set_index(int index) {

    _index = index;

    int offset = index * _projections.size();
    _offset.set_element(0, offset);
}


int ShadowSlot::get_index() const {
    return _index;
}

void ShadowSlot::cull(DisplayRegionCullCallbackData* data) {
    
    SceneSetup* setup = data->get_scene_setup();
    setup->set_initial_state(_state);

    data->upcall();
}


void ShadowSlot::draw(DisplayRegionDrawCallbackData* data) {

    SceneSetup* setup = data->get_scene_setup();

    if (setup == nullptr)
        return;

    data->upcall();

    DisplayRegion* dr = setup->get_display_region();

    // dr->set_active(false);
}



ShadowSlot::ShadowSlot(ShadowBuffer* host):
    _projections(host->_info.views, LMatrix4::ident_mat()),
    _regions(host->_info.views),
    _offset(1, 0),
    _index(0)
{

    GraphicsBuffer* buffer = host->get_buffer();
    GraphicsStateGuardian* gsg = buffer->get_gsg();

    _state.set_attrib(CullFaceAttrib::make(
        CullFaceAttrib::M_cull_counter_clockwise),
        RenderState::get_max_priority());


    for (int i=0; i<_regions.size(); ++i) {
        DisplayRegion* dr = buffer->make_mono_display_region();
        _regions[i] = dr;

        dr->set_cull_callback(_cull_callback);
        dr->set_draw_callback(_draw_callback);

        dr->set_lens_index(i);
        dr->set_target_tex_page(i);
        dr->set_clear_depth_active(true);
        dr->set_clear_depth(1.0f);
        dr->set_active(true);
    }
}


ShadowSlot* ShadowSlot::make(ShadowBuffer* host) {
    return new ShadowSlot(host);
}


ShadowSlot::~ShadowSlot() {

    for (DisplayRegion* dr: _regions)
        dr->get_window()->remove_display_region(dr);
}


void ShadowSlot::bind(LightBase* light) {
    
    for (DisplayRegion* dr: _regions) {
        dr->set_active(true);
        dr->set_camera(NodePath::any_path(light));
    }

    light->set_shadow_slot(_index);
}


void ShadowSlot::unbind(LightBase* light) {
    
    for (DisplayRegion* dr: _regions) {
        dr->set_active(false);
        dr->cleanup();
    }

    light->set_shadow_slot(-1);
}