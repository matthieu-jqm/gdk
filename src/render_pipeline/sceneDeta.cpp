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

#include <panda3d/displayRegion.h>
#include <panda3d/graphicsOutput.h>

#include "sceneData.h"


using namespace std;


DEFINE_TYPEHANDLE(SceneData)

SceneData::SceneData():
    _near_far(1, 0.0f),
    _view_scale(1, 0.0f),
    _resolution(1, 0)
{
    
}


void SceneData::update_data(SceneSetup* setup) {
    
    set_display_region(setup->get_display_region());
    set_viewport_size(setup->get_viewport_width(), setup->get_viewport_height());
    set_scene_root(setup->get_scene_root());
    set_camera_path(setup->get_camera_path());
    set_camera_node(setup->get_camera_node());
    set_lens(setup->get_lens());
    set_inverted(setup->get_inverted());
    set_view_frustum(setup->get_view_frustum());
    set_initial_state(setup->get_initial_state());
    set_camera_transform(setup->get_camera_transform());
    set_world_transform(setup->get_world_transform());
    set_cs_transform(setup->get_cs_transform());
    set_cs_world_transform(setup->get_cs_world_transform());

    const Lens* lens = get_lens();
    GraphicsOutput* output = setup->get_display_region()->get_window();

    _near_far[0].set(lens->get_near(), lens->get_far());

    _view_scale[0].set( tan(deg_2_rad(lens->get_hfov()*0.5f)),
                        tan(deg_2_rad(lens->get_vfov()*0.5f)));

    _resolution[0] = output->get_size();
}