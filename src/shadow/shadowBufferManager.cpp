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

#include "config_shadow.h"
#include "pointLightShadow.h"
#include "directionalLightShadow.h"
#include "shadowBufferManager.h"
#include "multiViewShadowBuffer.h"
#include "lightBase.h"

using namespace std;




ShadowBufferManager::ShadowBufferManager():
    _tag_name(shadow_main_cam_tag),
    _host(nullptr)
{
    add_buffer(PointLightShadow::get_info());
    add_buffer(DirectionalLightShadow::get_info());
    
    do_task_later([this] (AsyncTask* task) -> auto {
        determine_effective_views();

        for (Buffers::value_type& it: _buffers)
            it.second->update(this);
        return AsyncTask::DS_again;
    },
    "update-shadows",
    shadow_update_rate.get_value(),
    shadow_update_sort.get_value());
}


ShadowBuffer* ShadowBufferManager::get_buffer(TypeHandle type) const {

    Buffers::const_iterator it = _buffers.find(type);
    
    if (it != _buffers.end())
        return it->second;

    return nullptr;
}


void ShadowBufferManager::determine_effective_views() {
    
    _views.clear();
    
    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
    Thread* current_thread = Thread::get_current_thread();
    size_t num_win = engine->get_num_windows();


    for (size_t wi = 0; wi < num_win; ++wi) {
        GraphicsOutput* win = engine->get_window(wi);
        GraphicsStateGuardian* gsg = win->get_gsg();

        if (!win->is_active() || !gsg->is_active())
            continue;

        int num_display_regions = win->get_num_active_display_regions();

        for (int i = 0; i < num_display_regions; ++i)
            add_view(win->get_active_display_region(i));
    }
}


void ShadowBufferManager::add_view(DisplayRegion* region) {
    
    if (region == nullptr)
        return;

    NodePath camera = region->get_camera();

    if (camera.is_empty() || camera.node()->as_light() != nullptr)
        return;

    Camera* cam = DCAST(Camera, camera.node());

    if (cam->get_tag_state_key() == _tag_name && cam->is_active()) {

        NodePath root = cam->get_scene();
        if (root.is_empty())
            root = camera.get_top();

        PT(SceneSetup) view = new SceneSetup;
        view->set_scene_root(root);
        view->set_camera_node(cam);
        view->set_camera_path(camera);

        _views.push_back(move(view));
    }
}


ShadowBufferManager* ShadowBufferManager::get_global_ptr() {
    
    static Singleton<ShadowBufferManager> singleton(new ShadowBufferManager);
    return singleton;
}


void ShadowBufferManager::register_light(LightBase* light) {

    ShadowBuffer* buffer = get_buffer(light->get_type());
    buffer->register_light(light);
}


void ShadowBufferManager::unregister_light(LightBase* light) {
    
    TypeHandle type = light->get_type();
    Buffers::iterator it = _buffers.find(type);

    if (it != _buffers.end())
        it->second->unregister_light(light);
}


void ShadowBufferManager::set_tag_name(const std::string& name) {
    _tag_name = name;
}


GraphicsOutput* ShadowBufferManager::get_host() const {
    
    if (UNLIKELY(_host == nullptr)) {

        GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
        const string& name = shadow_host_name.get_value();

        int num_win = engine->get_num_windows();

        for (int i=0; i<num_win; ++i) {
            GraphicsOutput* buffer = engine->get_window(i);

            if (buffer->get_name() == name)
                _host = buffer;
        }
    }
    return _host;
}


void ShadowBufferManager::add_buffer(ShadowBufferInfo info) {

    GraphicsOutput* host = get_host();
    GraphicsStateGuardian* gsg = host->get_gsg();
    ShadowBuffer* buffer;

    if (gsg->has_extension("GL_ARB_geometry_shader4"))
        buffer = new MultiViewShadowBuffer(move(info));
    else
        buffer = new ShadowBuffer(move(info));

    _buffers[info.light_type] = buffer;

    buffer->setup(this);
}