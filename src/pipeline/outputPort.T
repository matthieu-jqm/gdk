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

template<class T>
DEFINE_TYPEHANDLE(OutputPort<T>)


template<class T>
OutputPort<T>::OutputPort(const std::string& name, PipelineNode* node):
        OutputPortBase(name, get_type_handle(T), node)
{

}

template<class T>
bool OutputPort<T>::connect(InputPort<T>& input) {
    return input.connect(*this);
}

template<class T>
T* OutputPort<T>::operator ->() const {
    return _data;
}

template<class T>
OutputPort<T>::operator T*() const {
    return _data;
}

template<class T>
OutputPort<T>& OutputPort<T>::operator =(T* data) {
    _data = data;
    return *this;
}

template<class T>
T* OutputPort<T>::get_data() const {
    return _data;
}

template<class T>
void OutputPort<T>::set_data(T* data) {
    _data = data;
}










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