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

#include "customEventHandler.h"

using namespace std;


EventHandlerBase::EventHandlerBase(EventQueue* queue): 
    _handler(this, queue)
{

}

EventHandlerBase::EventHandlerBase():
    _handler(this)
{
    
}


void EventHandlerBase::dispatch_event(const Event* event) {
    
    _handler.EventHandler::dispatch_event(event);
}


bool EventHandlerBase::add_hook(const string& event_name, 
                                EventFunction* function) 
{
    return _handler.add_hook(event_name, function);
}


bool EventHandlerBase::add_hook(const string& event_name, 
                EventCallbackFunction* function, void* data)
{
    return _handler.add_hook(event_name, function, data);
}


EventQueue* EventHandlerBase::get_event_queue() const {
    
    return &_handler._queue;
}


void CustomEventHandler::dispatch_event(const Event* event) {
    
    _impl->dispatch_event(event);
}


CustomEventHandler::CustomEventHandler( EventHandlerBase* impl, 
                                        EventQueue* queue):
    EventHandler(queue),
    _impl(impl)
{    

}

CustomEventHandler::CustomEventHandler(EventHandlerBase* impl):
    EventHandler(new EventQueue),
    _impl(impl)
{

}


CustomEventHandler::~CustomEventHandler() {

    delete &_queue;
}
