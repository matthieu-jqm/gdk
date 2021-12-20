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

#include <panda3d/displayRegionDrawCallbackData.h>
#include <panda3d/glgsg.h>

#include "lightBase.h"
#include "shaderLoader.h"
#include "render_utils.h"
#include "multiViewShadowBuffer.h"
#include "multiViewShadowSlot.h"

using namespace std;

DEFINE_TYPEHANDLE(MultiViewShadowSlot)


MultiViewShadowSlot::MultiViewShadowSlot(ShadowBuffer* host):
    ShadowSlot(host)
{
    MultiViewShadowBuffer* mvhost = DCAST(MultiViewShadowBuffer, host);
    GraphicsBuffer* buffer = mvhost->get_buffer();
    GraphicsStateGuardian* gsg = buffer->get_gsg();

    _region = buffer->make_mono_display_region();
    _region->set_cull_callback(_cull_callback);
    _region->set_draw_callback(_draw_callback);
    _region->set_active(false);
    _region->disable_clears();
    _region->set_target_tex_page(0);

    string geom, vert = GDK_SHADER_PATH "shadow/multiViewShadows";

    if (!gsg->has_extension("GL_AMD_vertex_shader_layer"))
        geom = GDK_SHADER_PATH "shadow/multiViewShadows";


    _state.set_shader(  ShaderLoader::load(vert, "", geom),
                        RenderState::get_max_priority());

    _state.set_shader_input("u_projections", _projections);
    _state.set_shader_input("u_layerOffset", _offset);
    _state.set_instance_count(host->_info.views);
}


void MultiViewShadowSlot::bind(LightBase* light) {

    for (DisplayRegion* dr: _regions) {
        dr->set_clear_depth_active(true);
        dr->set_active(true);
    }

    _region->set_camera(NodePath::any_path(light));
    _region->set_active(true);
    light->set_shadow_slot(_index);
}

void MultiViewShadowSlot::unbind(LightBase* light) {

    for (DisplayRegion* dr: _regions)
        dr->set_active(false);

    _region->cleanup();
    _region->set_active(false);
    light->set_shadow_slot(-1);
}


void MultiViewShadowSlot::draw(DisplayRegionDrawCallbackData* data) {

    SceneSetup* setup = data->get_scene_setup();
    DisplayRegion* dr = setup->get_display_region();
    GraphicsOutput* buffer = dr->get_window();
    
    GraphicsStateGuardian* gsg = buffer->get_gsg();

    if (gsg->is_of_type(CLP(GraphicsStateGuardian)::get_class_type())) {
        CLP(GraphicsStateGuardian)* glgsg = DCAST(CLP(GraphicsStateGuardian), gsg);
        int num_views = _projections.size();

        Texture* target = buffer->get_texture(0);
        if (target != nullptr) {
            TextureContext* tc = target->prepare_now(0,
                        glgsg->get_prepared_objects(), gsg);

            CLP(TextureContext)* gtc = DCAST(CLP(TextureContext), tc);
            LVector2i size = dr->get_pixel_size();
            vector_uchar data = target->get_clear_data();

            if (glgsg->_glClearTexSubImage) {
                glgsg->_glClearTexSubImage(gtc->_index, 0, 0, 0,
                    _index * num_views, size.get_x(), size.get_y(), num_views,
                    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data.data());
            }
        }
    }

    data->upcall();
}