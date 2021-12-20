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

#include "graphicsCallback.h"
#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/displayRegionDrawCallbackData.h>

#include "callbacks.h"


using namespace std;
using RenderCallbackData = CallbackGraphicsWindow::RenderCallbackData;

DEFINE_TYPEHANDLE(GraphicsCallback::Callback)


#define make_callback(method) _make_callback(this, &GraphicsCallback::method)


template<class T>
GraphicsCallback::Callback* _make_callback(GraphicsCallback* object, 
                                        void (GraphicsCallback::*method)(T*))
{

    auto func = [method](GraphicsCallback* object, CallbackData* data) {
        T* cdata = DCAST(T, data);

        if (LIKELY(object != nullptr))
            (object->*method)(cdata);
        else
            callback_garbage_collect(cdata);
    };

    return new GraphicsCallback::Callback(object, func);
}



void callback_garbage_collect(CallbackData*) {

}

void callback_garbage_collect(RenderCallbackData* data) {

    data->get_window()->clear_render_callback();
}

void callback_garbage_collect(DisplayRegionCullCallbackData* data) {

    if (SceneSetup* setup = data->get_scene_setup())
        setup->get_display_region()->clear_cull_callback();
}

void callback_garbage_collect(DisplayRegionDrawCallbackData* data) {

    if (SceneSetup* setup = data->get_scene_setup())
        setup->get_display_region()->clear_cull_callback();
}



GraphicsCallback::Callback::Callback(GraphicsCallback* object, 
                                        function<Func> func):
    _object(object),
    _func(func)
{
    
}   

void GraphicsCallback::Callback::mark_dirty() {

    _object = nullptr;
}

void GraphicsCallback::Callback::do_callback(CallbackData* data) {

    _func(_object, data);
}


GraphicsCallback::~GraphicsCallback() {
    
    _draw_callback->mark_dirty();
    _cull_callback->mark_dirty();
    _render_callback->mark_dirty();
}


void GraphicsCallback::cull(DisplayRegionCullCallbackData* data) {
    data->upcall();    
}

void GraphicsCallback::draw(DisplayRegionDrawCallbackData* data) {
    data->upcall();
}

void GraphicsCallback::render(RenderCallbackData* data) {
    data->upcall();
}


GraphicsCallback::GraphicsCallback():
    _draw_callback(make_callback(draw)),
    _cull_callback(make_callback(cull)),
    _render_callback(make_callback(render))
{
    

}
