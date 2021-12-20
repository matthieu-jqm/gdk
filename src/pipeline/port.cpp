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

#include "port.h"

using namespace std;


DEFINE_TYPEHANDLE(Port)



TypeHandle Port::get_data_type() const {
    return _type;
}


string Port::get_name() const {
    return _name;
}


PipelineNode* Port::get_node() const {
    return _node;
}


Port::Port(const string& name, TypeHandle type, PipelineNode* node): 
    _name(name), 
    _type(type), 
    _node(node)
{
    
}


Port::Port(const Port& copy): 
    _name(copy._name),
    _type(copy._type),
    _node(copy._node)
{
    
}
