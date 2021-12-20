
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

#ifndef __APPLICATIONFRAMEWORK_H__
#define __APPLICATIONFRAMEWORK_H__


#include <panda3d/asyncTaskManager.h>
#include <panda3d/graphicsEngine.h>
#include <panda3d/graphicsPipe.h>

#include "callbacks.h"


class ApplicationRunner;


class ApplicationFramework: public TypedObject, public TaskPool {

    REGISTER_TYPE("ApplicationFramework", TypedObject)

public:
    enum ExitStatus {
        ES_none = -1,
        ES_success = EXIT_SUCCESS,
        ES_failure = EXIT_FAILURE,
    };

    virtual ~ApplicationFramework() = default;

    ExitStatus get_exit_status() const;
    void set_exit_status(ExitStatus status);

    GraphicsPipe* get_default_pipe();

    GraphicsOutput* open_window(const std::string& name,
        FrameBufferProperties fb_prop=FrameBufferProperties::get_default(),
        WindowProperties win_prop=WindowProperties::get_default());

private:
    LightMutex _lock;
    ExitStatus _exit_flag;

    PT(GraphicsPipe) _default_pipe;
    PT(AsyncTaskManager) _task_mgr;

protected:
    ApplicationFramework();

    virtual void start();
    virtual void cleanup();
    virtual void main_loop() const;
    virtual void do_frame(Thread* current_thread) const;

    virtual void make_default_pipe();

    friend class ApplicationRunner;
};


class ApplicationRunner {

public:
    ApplicationRunner(ApplicationFramework* app);
    ~ApplicationRunner();

    ApplicationFramework::ExitStatus exec() const;

private:
    ApplicationFramework* _app;
};

#endif // __APPLICATIONFRAMEWORK_H__