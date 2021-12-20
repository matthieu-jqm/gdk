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
DEFINE_TYPEHANDLE(Property<T>)


template<class T>
Property<T>::Property(const std::string& name, const T& defaultv):
    PropertyBase(name, get_type_handle(T)),
    _value(defaultv)
{

}


template<class T>
INLINE T Property<T>::get_value() const {
    return _value;
}


template<class T>
INLINE void Property<T>::set_value(const T& value) {
    _value = value;
}


template<class T>
INLINE Property<T>& Property<T>::operator =(const T& value) {
    _value = value;
    return *this;
}


template<class T>
INLINE Property<T>::operator T() const {
    return _value;
}


template<class T>
void Property<T>::fillin(DatagramIterator& scan, BamReader* manager) {
    generic_read_datagram(_value, scan);    
}


template<class T>
void Property<T>::write_datagram(BamWriter* manager, Datagram& me) {
    generic_write_datagram(me, _value);
}



template<class T, class... Args>
Property<T>& Serializable::make_property(const std::string& name, Args... args) {

    std::string label = std::replace(name.begin(), name.end(), '_', ' ');

    Property<T>* property = new Property<T>(label, args...);
    _properties[label] = property;

    return *property;
}
