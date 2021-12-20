
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
DEFINE_TYPEHANDLE(InputPort<T>)


template<class T> 
InputPort<T>::InputPort(const std::string& name, PipelineNode* node): 
        InputPortBase(name, get_type_handle(T), node)
{

} 

template<class T>
bool InputPort<T>::connect(OutputPort<T>& output) { 
    return InputPortBase::connect(&output); 
} 

template<class T>
T* InputPort<T>::get_data() const {
    if (PT(OutputPort<T>) endpoint = DCAST(OutputPort<T>, _endpoint.lock()))
        return endpoint->get_data();

    return nullptr;
}

template<class T>
bool InputPort<T>::has_data() const {
    return get_data() == nullptr;
}

template<class T>
T* InputPort<T>::operator ->() const {
    return get_data();
}

template<class T>
InputPort<T>::operator bool() const {
    return get_data() != nullptr;
}

template<class T>
InputPort<T>::operator T*() const {
    return get_data();
}
