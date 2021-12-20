
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

#ifndef __INPUTPORT_H__
#define __INPUTPORT_H__

#include <panda3d/weakPointerTo.h>
#include "port.h"

class OutputPortBase;


class InputPortBase: public Port {

    REGISTER_TYPE("InputPortBase", Port)

public:
    InputPortBase(const std::string& name, TypeHandle type, PipelineNode* node);
    ~InputPortBase();

    bool connect(PipelineNode* parent, const std::string& name);
    bool connect(OutputPortBase* port);
    void disconnect();

    OutputPortBase* get_endpoint() const;

protected:
    WPT(OutputPortBase) _endpoint;

    friend class OutputPortBase;
    friend class PipelineNode;
};


template<class T>
class OutputPort;


template<class T> 
class InputPort: public InputPortBase { 

    REGISTER_TEMPLATE("InputPort", InputPortBase)

public:
    InputPort() = default;
    InputPort(const std::string& name, PipelineNode* node);
    ~InputPort() = default;
 
    bool connect(OutputPort<T>& output);
    T* get_data() const;
    bool has_data() const;

    T* operator ->() const;
    operator bool() const;
    operator T*() const;

private:
    friend class OutputPort<T>;
};


#include "inputPort.T"

#endif // __INPUTPORT_H__









// template<class T> 
// class InputPort { 

// public:
//     InputPort(const std::string& name, PipelineNode* node): 
//         _port(new InputPortBase(name, get_type_handle(T), node)) {} 

//     ~InputPort() = default;
 
//     bool connect(OutputPort<T>& output) { 
//         return _port->connect(output._port); 
//     } 

//     void disconnect() { 
//         _port->disconnect(); 
//     }

//     T* get_data() const {
//         return static_cast<T*>(_port->get_data());
//     }

//     bool has_data() const {
//         return _port->has_data();
//     }

// private:
//     PT(InputPortBase) _port;

//     friend class OutputPort<T>;
// };