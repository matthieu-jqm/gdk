

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

#ifndef __GRAPHICSCALLBACK_H__
#define __GRAPHICSCALLBACK_H__

#include <panda3d/callbackObject.h>
#include <panda3d/callbackGraphicsWindow.h>

#include "utils.h"


class DisplayRegionCullCallbackData;
class DisplayRegionDrawCallbackData;


class GraphicsCallback {

public:
    using RenderCallbackData = CallbackGraphicsWindow::RenderCallbackData;

    GraphicsCallback();
    virtual ~GraphicsCallback();

    virtual void cull(DisplayRegionCullCallbackData* data);
    virtual void draw(DisplayRegionDrawCallbackData* data);
    virtual void render(RenderCallbackData* data);


    class Callback: public CallbackObject {
    
        REGISTER_TYPE("GraphicsCallback::Callback", Callback)

    public:
        ALLOC_DELETED_CHAIN(Callback);

        typedef void Func(GraphicsCallback*, CallbackData*);

        Callback(GraphicsCallback* object, std::function<Func> func);
        ~Callback() = default;

        void mark_dirty();

    private:
        void do_callback(CallbackData* data) override;

        GraphicsCallback* _object;
        std::function<Func> _func;
    };

    PT(Callback) _render_callback;
    PT(Callback) _cull_callback;
    PT(Callback) _draw_callback;
};

#endif // __GRAPHICSCALLBACK_H__