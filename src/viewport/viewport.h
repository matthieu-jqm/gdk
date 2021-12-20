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

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__


#include <panda3d/displayRegion.h>
#include "utils.h"


class ViewportManager;


class Viewport final : public DisplayRegion  {

    REGISTER_TYPE("Viewport", DisplayRegion)

public:
    ~Viewport();

    ViewportManager* get_manager() const;
    void set_dimensions(int i, const LVecBase4& dimensions) override;

    void release(bool update=true);
    void stash();

    void set_event_queue(EventQueue* queue);
    void set_event_name(const std::string& name);

    using DisplayRegion::set_dimensions;

private:
    Viewport(ViewportManager* manager);

    std::string _event_name;
    EventQueue* _event_queue;

    ViewportManager* _mgr;

    friend class ViewportManager;
    friend class ReferenceCount;
};

#endif // __VIEWPORT_H__


    // typedef void _EventCallback(Viewport*);
    // std::function<_EventCallback> _dimension_changed;
    // typedef void EventCallback(Viewport*, void*);
    // void on_dimension_changed(EventCallback* func, void* data);