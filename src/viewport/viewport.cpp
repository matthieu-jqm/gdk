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

#include <panda3d/eventQueue.h>

#include "viewportManager.h"
#include "viewport.h"

using namespace std;

DEFINE_TYPEHANDLE(Viewport)


Viewport::Viewport(ViewportManager* manager): 
    DisplayRegion(manager->get_buffer(), LVector4(0,1,0,1)), _mgr(manager)
{
    
}


ViewportManager* Viewport::get_manager() const {
    
    return _mgr;
}


void Viewport::set_dimensions(int i, const LVecBase4& dimensions) {
    
    DisplayRegion::set_dimensions(i, dimensions);

    if (!_event_name.empty()) {
        
        EventQueue* queue = _event_queue;
        if (queue == nullptr)
            queue = EventQueue::get_global_event_queue();

        Event* event = new Event(_event_name);
        event->add_parameter(this);
        queue->queue_event(event);
    }
}


void Viewport::release(bool update) {
    
    _mgr->release_viewport(this, update);
}


void Viewport::stash() {

    _mgr->stash_viewport(this);    
}


void Viewport::set_event_queue(EventQueue* queue) {
    
    _event_queue = queue;
}


void Viewport::set_event_name(const string& name) {

    _event_name = name;    
}


Viewport::~Viewport() {
    
    release();
}



// void Viewport::on_dimension_changed(EventCallback* func, void* data) {
    
//     _dimension_changed = bind(func, placeholders::_1, data);
// }