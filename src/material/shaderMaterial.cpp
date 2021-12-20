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

#include <panda3d/shaderAttrib.h>
#include <panda3d/shader.h>

#include "shaderMaterial.h"


using namespace std;

DEFINE_TYPEHANDLE(PropertyBase)
DEFINE_TYPEHANDLE(ShaderMaterial)



ShaderMaterial::ShaderMaterial(const std::string& name):
    Namable(name)
{
    _attrib = DCAST(ShaderAttrib, ShaderAttrib::make());
}


void ShaderMaterial::fillin(DatagramIterator& scan, BamReader* manager) {
    
    Serializable::fillin(scan, manager);
    set_name(scan.get_string());
}


void ShaderMaterial::write_datagram(BamWriter* manager, Datagram& me) {

    Serializable::write_datagram(manager, me);
    me.add_string(get_name());    
}


Null ShaderMaterial::make_flag(int flag) {
    set_flag(flag, true);
    return ::Null();
}


void ShaderMaterial::set_shader_input(const ShaderInput& input) {

    const ShaderInput& data = _attrib->get_shader_input(input.get_name());

    if (data.get_value_type() == ShaderInput::M_invalid)
        _attrib = DCAST(ShaderAttrib, _attrib->set_shader_input(input));
    else
        const_cast<ShaderInput&>(data) = input;
}

ShaderMaterial::operator const ShaderAttrib*() const {
    return _attrib;
}

const ShaderAttrib* ShaderMaterial::get_attrib() const {
    return _attrib;
}

void ShaderMaterial::set_flag(int flag, bool value) {
    _attrib = DCAST(ShaderAttrib, _attrib->set_flag(flag, value));
}

