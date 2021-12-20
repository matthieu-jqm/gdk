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


#include <panda3d/inputDeviceNode.h>
#include <panda3d/dataNodeTransmit.h>

#include "inputInterfaceManager.h"
#include "inputDeviceInterface.h"
#include "utils.h"

using namespace std;
using DeviceClass=InputDevice::DeviceClass;


InputDeviceInterface::InputDeviceInterface(DeviceClass device_class):
    InputDeviceNode(nullptr, "input-interface"),
    _device_class(device_class)
{
    InputInterfaceManager* mgr = InputInterfaceManager::get_global_ptr();

    set_device(mgr->acquire_device(device_class));
}


InputDeviceInterface::InputDeviceInterface(const InputDeviceInterface& copy):
    InputDeviceInterface(copy._device_class)
{
        
}


InputDeviceInterface::~InputDeviceInterface() {

    InputInterfaceManager* mgr = InputInterfaceManager::get_global_ptr();
    InputDevice* device = get_device();

    if (device != nullptr)
        mgr->release_device(device);
}


bool InputDeviceInterface::is_valid() const {

    InputDevice* device = get_device();

    return device != nullptr && device->is_connected();
}


InputDevice::DeviceClass InputDeviceInterface::get_device_class() const {
    
    return _device_class;
}


void InputDeviceInterface::on_connect(DeviceCallback* func, void* data) {
    
    _connect_callback = bind(func, placeholders::_1, data);
}


void InputDeviceInterface::on_disconnect(DeviceCallback* func, void* data) {
    
    _disconnect_callback = bind(func, placeholders::_1, data);
}


void InputDeviceInterface::set_device(InputDevice* device) {
    
    if (get_device() == device)
        return;
    
    InputDeviceNode::set_device(device);

    if (device == nullptr) {
        if (_disconnect_callback)
            _disconnect_callback(this);
    } else {
        if (_connect_callback)
            _connect_callback(this);
    }
}


// void InputDeviceInterface::do_transmit_data(DataGraphTraverser *, 
//     const DataNodeTransmit &, DataNodeTransmit &output)
// {

//     InputInterfaceManager* mgr = InputInterfaceManager::get_global_ptr();

   
//     InputDevice* device = get_device();

    // if (device == nullptr) {
    //     set_device(mgr->acquire_device(_device_class));

    //     if (get_device() != nullptr)
    //         _disconnect_callback(this);

    // } else if (!device->is_connected()) {
        
    //     mgr->release_device(device);
    //     set_device(nullptr);
    //     _connect_callback(this);
    // }

    // else {
    //     device->poll();

    //     if (!device->has_button_event())
    //         return;
        
    //     PT(ButtonEventList) bel = device->get_button_events();

    //     output.set_data(_button_events_output, EventParameter(bel));
    // }
// }


// void InputDeviceInterface::set_device(InputDevice* device) {
    
//     _device = device;
// }


// InputDevice* InputDeviceInterface::get_device() const {
    
//     return _device;
// }



// void InputDeviceInterface::on_event(EventCallback* func, void* data)
// {    
//     _event_callback._func = func;
//     _event_callback._data = data;
// }