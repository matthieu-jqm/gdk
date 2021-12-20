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

#include <panda3d/pandabase.h>
#include <panda3d/pandaSystem.h>
#include <panda3d/graphicsPipeSelection.h>
#include <panda3d/pStatClient.h>

#include "shaderLoader.h"
#include "applicationFramework.h"
#include "config_gdk.h"

using namespace std;


DEFINE_TYPEHANDLE(ApplicationFramework)



ApplicationFramework::ApplicationFramework(): 
    _task_mgr(AsyncTaskManager::get_global_ptr()),
    _exit_flag(ES_none)
{

#ifdef LINK_ALL_STATIC
    // If we're statically linking, we need to explicitly link with at least one
    // of the available renderers.
    #if defined(HAVE_GL)
    extern void init_libpandagl();
    init_libpandagl();
    #elif defined(HAVE_DX9)
    extern EXPCL_PANDADX void init_libpandadx9();
    init_libpandadx9();
    #elif defined(HAVE_TINYDISPLAY)
    extern EXPCL_TINYDISPLAY void init_libtinydisplay();
    init_libtinydisplay();
    #endif

    // Ensure the animation subsystem is available.
    extern EXPCL_PANDA_CHAR void init_libchar();
    init_libchar();

    // We also want the egg loader.
    #ifdef HAVE_EGG
    init_libpandaegg();
    #endif
#endif

    make_default_pipe();

#ifndef NDEBUG

#endif
    // extern EXPCL_PANDA_PNMIMAGETYPES void init_libpnmimagetypes();
    // init_libpnmimagetypes();
}


GraphicsOutput* ApplicationFramework::open_window(const string& name,
                                FrameBufferProperties fb_prop,
                                WindowProperties win_prop)
{
    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();

    GraphicsOutput* window = engine->make_output(get_default_pipe(), 
                            name, 50, fb_prop, win_prop, 
                            GraphicsPipe::BF_require_window,
                            nullptr);

    engine->open_windows();

    if (window != nullptr && !window->is_valid()) {
        engine->remove_window(window);
        window = nullptr;
    }

    return window;
}


void ApplicationFramework::make_default_pipe() {

    auto selection = GraphicsPipeSelection::get_global_ptr();

    _default_pipe = selection->make_default_pipe();

    if (_default_pipe == nullptr) {
        gdk_error("No graphics pipe is available!\n"
            << "Your Config.prc file must name at least one valid panda display\n"
            << "library via load-display or aux-display.");
    }
}




void ApplicationFramework::main_loop() const {

    Thread* current_thread = Thread::get_current_thread();

    while (_exit_flag == ES_none)
        do_frame(current_thread);
}


void ApplicationFramework::cleanup() {

}

void ApplicationFramework::start() {

#ifndef NDEBUG
    if (PStatClient::is_connected())
        PStatClient::disconnect();

    if (!PStatClient::connect("", -1))
        gdk_info("Could not connect to PStat server.");

    ShaderLoader::start();
#endif
}


void ApplicationFramework::do_frame(Thread* current_thread) const {

    _task_mgr->poll();
}


GraphicsPipe* ApplicationFramework::get_default_pipe() {
    
    if (_default_pipe == nullptr)
        make_default_pipe();

    return _default_pipe;
}



ApplicationFramework::ExitStatus ApplicationFramework::get_exit_status() const {

    LightMutexHolder holder(_lock);
    return _exit_flag;
}


void ApplicationFramework::set_exit_status(ExitStatus status) {

    LightMutexHolder holder(_lock);
    _exit_flag = status;
}




ApplicationRunner::ApplicationRunner(ApplicationFramework* app):
    _app(app) {}


ApplicationRunner::~ApplicationRunner() {

    _app->cleanup();
    Thread::prepare_for_exit();
}


ApplicationFramework::ExitStatus ApplicationRunner::exec() const {

    _app->start();
    gdk_info("application started");

    _app->main_loop();
    gdk_info("application finished");

    if (_app->_exit_flag == ApplicationFramework::ES_none)
        return ApplicationFramework::ES_success;

    return _app->_exit_flag;
}