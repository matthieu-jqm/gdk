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

#ifndef __OUTPUTPORT_H__
#define __OUTPUTPORT_H__

#include "port.h"
#include <unordered_set>

class InputPortBase;


class OutputPortBase: public Port {

    REGISTER_TYPE("OutputPortBase", Port)

public:
    OutputPortBase(const std::string& name, TypeHandle type, PipelineNode* node);
    ~OutputPortBase() = default;

    void connected();
    void disconnected();
};


template<class T>
class InputPort;


template<class T>
class OutputPort: public OutputPortBase {

    REGISTER_TEMPLATE("OutputPort", OutputPortBase)

public:
    OutputPort() = default;
    OutputPort(const std::string& name, PipelineNode* node);
    ~OutputPort() = default;

    bool connect(InputPort<T>& input);

    T* operator ->() const;
    operator T*() const;
    OutputPort<T>& operator =(T* data);

    T* get_data() const;
    void set_data(T* data);
private:
    PointerTo<T> _data;
};

#include "outputPort.T"

#endif // __OUTPUTPORT_H__









// template<class T>
// class OutputPort {

// public:
//     OutputPort(const std::string& name, PipelineNode* node): 
//         _port(new OutputPortBase(name, get_type_handle(T), node)) {}

//     ~OutputPort() = default;

//     bool connect(const InputPort<T>& input) {
//         return _port->connect(input._port);
//     }

//     void disconnect(const InputPort<T>& input) {
//         _port->disconnect(input._port);
//     }

//     T* get_data() const {
//         return static_cast<T*>(_port->get_data());
//     }

//     void set_data(T* data) {
//         _port->set_data((void*)data);
//     }

// private:
//     PT(OutputPortBase) _port;
// };





    // struct Hash {
    //     size_t operator() (const PT(InputPortBase)& port) const {
    //         return port.get_hash();
    //     }
    // };

    // typedef std::unordered_set<PT(InputPortBase), Hash> Endpoints;
    // Endpoints _endpoints;