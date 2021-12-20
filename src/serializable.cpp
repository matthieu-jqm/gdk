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

#include "serializable.h"

using namespace std;


DEFINE_TYPEHANDLE(Serializable)


PropertyBase::PropertyBase(const std::string& name, TypeHandle type):
    Namable(name),
    _data_type(type)
{
    
}


void PropertyBase::fillin(DatagramIterator& scan, BamReader* manager) {
    
}


void PropertyBase::write_datagram(BamWriter* manager, Datagram& me) {
    
}


TypeHandle PropertyBase::get_data_type() const {
    return _data_type;    
}


const Serializable::Properties& Serializable::get_properties() const {
    return _properties;
}


void Serializable::fillin(DatagramIterator& scan, BamReader* manager) {

    for (const Properties::value_type& it : _properties)
        it.second->fillin(scan, manager);
}


void Serializable::write_datagram(BamWriter* manager, Datagram& me) {

    for (const Properties::value_type& it : _properties)
        it.second->write_datagram(manager, me);
}


void generic_read_datagram(TypedWritable* value, DatagramIterator& scan) {

}
