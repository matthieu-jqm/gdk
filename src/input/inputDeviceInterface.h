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

#ifndef __INPUTDEVICEINTERFACE_H__
#define __INPUTDEVICEINTERFACE_H__


#include <panda3d/pointerTo.h>
#include "callbacks.h"
#include <panda3d/inputDeviceNode.h>

class InputDevice;
class InputInterfaceManager;


class InputDeviceInterface: public InputDeviceNode {

public:

    enum EventType {
        connected = 0,
        disconnected,
        button_event
    };

    InputDeviceInterface(InputDevice::DeviceClass device_class);
    InputDeviceInterface(const InputDeviceInterface& copy);
    ~InputDeviceInterface();

    InputDevice::DeviceClass get_device_class() const;
    void get_device_class(InputDevice::DeviceClass type) const;

    bool is_valid() const;

    typedef void DeviceCallback(InputDeviceInterface*, void*);

    void on_connect(DeviceCallback* func, void* data=nullptr);
    void on_disconnect(DeviceCallback* func, void* data=nullptr);

private:
    void set_device(InputDevice* device);

    InputDevice::DeviceClass _device_class;
    int _button_events_output;

    typedef void _DeviceCallback(InputDeviceInterface*);

    std::function<_DeviceCallback> _connect_callback;
    std::function<_DeviceCallback> _disconnect_callback;

    friend class InputInterfaceManager;
};

#endif // __INPUTDEVICEINTERFACE_H__



// protected:
//     virtual void do_transmit_data(DataGraphTraverser *, 
//                 const DataNodeTransmit &, DataNodeTransmit &output);
    // typedef void EventCallback(InputDeviceInterface*,const ButtonEvent&,void*);
    // void on_event(EventCallback* func, void* data=nullptr);
    // CallbackHandler<EventCallback> _event_callback; 