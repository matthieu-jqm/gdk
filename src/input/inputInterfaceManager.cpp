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


#include <panda3d/inputDeviceManager.h>
#include "inputDeviceInterface.h"
#include "inputInterfaceManager.h"
#include "callbacks.h"


using namespace std;
using DeviceClass = InputDevice::DeviceClass;


Singleton<InputInterfaceManager> InputInterfaceManager::_global_ptr;


InputInterfaceManager::InputInterfaceManager():
    _mgr(InputDeviceManager::get_global_ptr()) {
    
    EventHandler* handler = EventHandler::get_global_event_handler();

    register_hook(callback(connect_hook), "connect-device");
    register_hook(callback(disconnect_hook), "disconect-device");
}


void InputInterfaceManager::connect_hook(const Event* event) {
    
    nassertv(event->get_num_parameters() > 0)
    
    EventParameter param = event->get_parameter(0);
    InputDevice* device = DCAST(InputDevice, param.get_ptr());
    
    // a new device has been detected, let's assign it to a pending interface
    for (InputDeviceInterface* interface: _interfaces) {

        if (!interface->is_valid()) {
            DeviceClass type = device->get_device_class();

            if (interface->get_device_class() == type) {
                if (interface->get_device() == nullptr)
                    _types_count[size_t(type)]++;
                
                interface->set_device(device);
                break;
            }
        }
    }
}


void InputInterfaceManager::disconnect_hook(const Event* event) {

    nassertv(event->get_num_parameters() > 0)
    
    EventParameter param = event->get_parameter(0);
    InputDevice* device = DCAST(InputDevice, param.get_ptr());

    for (InputDeviceInterface* interface: _interfaces) {

        if (interface->get_device() == device) {
            DeviceClass type = device->get_device_class();
            _types_count[size_t(type)]--;

            interface->set_device(nullptr);
            break;
        }
    }
}



bool InputInterfaceManager::has_device(InputDevice* device) const {

    int num_interface = _interfaces.size();

    DeviceInterfaceSet::const_iterator it = _interfaces.begin();

    for (; it != _interfaces.end(); ++it) {
        InputDeviceInterface* interface = it->p();
        if (interface->get_device() == device)
            return true;
    }
    return false;
}


void InputInterfaceManager::release_device(InputDevice* device) {
    
    nassertv(has_device(device))
    _types_count[size_t(device->get_device_class())]--;
}


InputInterfaceManager::~InputInterfaceManager() {
    
    EventHandler* handler = EventHandler::get_global_event_handler();

    handler->remove_hooks_with(this);
}



InputDevice* InputInterfaceManager::acquire_device(DeviceClass type) {

    if (get_num_available_devices(type)) {

        InputDeviceSet devs = _mgr->get_devices(type);
        int num_devices = devs.size();

        for (int i=0; i<num_devices; ++i) {
            InputDevice* device = devs[i];

            if (!has_device(device)) {
                _types_count[size_t(type)]++;
                return device;
            }
        }
    }
    return nullptr;
}


InputDeviceInterface* InputInterfaceManager::get_interface(size_t id) const {
    
    nassertr(id < _interfaces.size(), nullptr)
    return _interfaces[id];
}


InputDeviceInterface* InputInterfaceManager::acquire_interface(DeviceClass type) 
{
    InputDeviceInterface* interface = new InputDeviceInterface(type);
    _interfaces.push_back(interface);

    return interface;
}


void InputInterfaceManager::release_interface(InputDeviceInterface* interface) {
    
    DeviceInterfaceSet::iterator found = find(_interfaces.begin(), 
                                        _interfaces.end(), interface);

    
    nassertv(found != _interfaces.end())
    _interfaces.erase(found);
}


size_t InputInterfaceManager::get_num_interface() const {

    return _interfaces.size();
}


size_t InputInterfaceManager::get_num_available_devices() const {

    size_t num_used = accumulate(_types_count.begin(), _types_count.end(), 0);
    return _mgr->get_devices().size() - num_used;
}


size_t InputInterfaceManager::get_num_available_devices(DeviceClass type) const {

    size_t num_used = _types_count[size_t(type)];
    return _mgr->get_devices(type).size() - num_used;
}


InputInterfaceManager* InputInterfaceManager::get_global_ptr() {

    if (UNLIKELY(!_global_ptr))
        _global_ptr = new InputInterfaceManager;

    return _global_ptr;    
}



// size_t InputInterfaceManager::get_num_interface(bool has_device) const {
    
//     size_t counter = 0;

//     if (has_device) {
//         DeviceInterfaceSet::const_iterator it = _interfaces.begin();

//         for (; it != _interfaces.end(); ++it) {
//             InputDeviceInterface* interface = it->p();
//             if (interface->get_device() != nullptr)
//                 ++counter;
//         }
//     } else
//         counter = _interfaces.size();
    
//     return counter;
// }


// size_t InputInterfaceManager::get_num_interface(DeviceClass device_class, 
//                                             bool has_device) const 
// {
    
//     size_t num_interface = _interfaces.size();
//     size_t counter = 0;

//     if (has_device)
//         counter = _types_count[size_t(device_class)];
//     else {
//         DeviceInterfaceSet::const_iterator it = _interfaces.begin();

//         for (; it != _interfaces.end(); ++it) {
//             InputDeviceInterface* interface = it->p();
//             if (interface->get_device_class() == device_class)
//                 ++counter;
//         }
//     }

//     return counter;
// }
