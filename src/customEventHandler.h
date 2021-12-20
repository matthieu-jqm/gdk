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

#ifndef __CUSTOMEVENTHANDLER_H__
#define __CUSTOMEVENTHANDLER_H__


#include <panda3d/eventHandler.h>

class EventHandlerBase;


class CustomEventHandler final: public EventHandler {

public:
    ~CustomEventHandler();

    void dispatch_event(const Event* event) override;

private:
    CustomEventHandler(EventHandlerBase* impl, EventQueue* queue);
    CustomEventHandler(EventHandlerBase* impl);
    EventHandlerBase* _impl;

    friend class EventHandlerBase;
};


class EventHandlerBase {

public:
    using EventCallbackFunction = EventHandler::EventCallbackFunction;
    using EventFunction = EventHandler::EventFunction;

    EventHandlerBase();
    explicit EventHandlerBase(EventQueue* queue);
    ~EventHandlerBase() = default;

    virtual void dispatch_event(const Event* event);

    bool add_hook(const std::string& event_name, EventFunction* function);
    bool add_hook(const std::string& event_name, EventCallbackFunction* function,
                void* data);

    EventQueue* get_event_queue() const;

protected:
    CustomEventHandler _handler;
};



#endif // __CUSTOMEVENTHANDLER_H__