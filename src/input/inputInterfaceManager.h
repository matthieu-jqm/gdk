
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

#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <panda3d/pointerTo.h>
#include <panda3d/inputDevice.h>
#include "inputDeviceInterface.h"

class InputDeviceInterface;
class InputDevice;

class InputInterfaceManager {

public:
    ~InputInterfaceManager();
    
    InputDeviceInterface* get_interface(size_t id) const;

    InputDeviceInterface* acquire_interface(InputDevice::DeviceClass type);
    void release_interface(InputDeviceInterface* interface);

    size_t get_num_interface() const;

    size_t get_num_available_devices() const;
    size_t get_num_available_devices(InputDevice::DeviceClass type) const;

    bool has_device(InputDevice* device) const;
    InputDevice* acquire_device(InputDevice::DeviceClass type);
    void release_device(InputDevice* device);

    static InputInterfaceManager* get_global_ptr();

private:
    InputInterfaceManager();
    InputInterfaceManager(const InputInterfaceManager& copy);

    void connect_hook(const Event* event);
    void disconnect_hook(const Event* event);


    static Singleton<InputInterfaceManager> _global_ptr;

    typedef pvector<PT(InputDeviceInterface)> DeviceInterfaceSet;
    typedef std::array<size_t, 12> InterfaceTypes;

    DeviceInterfaceSet _interfaces;
    InterfaceTypes _types_count;

    InputDeviceManager* _mgr;
};

#endif // __INPUTMANAGER_H__


    // size_t get_num_interface(bool has_device=false) const;
    // size_t get_num_interface(InputDevice::DeviceClass type, 
    //                     bool has_device=false) const;