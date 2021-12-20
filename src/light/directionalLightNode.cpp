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

#include <panda3d/orthographicLens.h>
#include <panda3d/omniBoundingVolume.h>

#include "directionalLightNode.h"


DEFINE_TYPEHANDLE(DirectionalLightNode)



DirectionalLightNode::DirectionalLightNode(const std::string& name):
    LightBase(name, new OrthographicLens)
{
    set_internal_bounds(new OmniBoundingVolume);
}


DirectionalLightNode::DirectionalLightNode(const DirectionalLightNode& copy):
    LightBase(copy),
    _cycler(copy._cycler)
{
    
}


void DirectionalLightNode::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


int DirectionalLightNode::get_class_priority() const {
    return CP_directional_priority;
}


void DirectionalLightNode::set_angular_diameter(float diameter) {
    
    CDWriter cdata(_cycler);
    cdata->_angular_diameter = diameter;
}


float DirectionalLightNode::get_angular_diameter() const {
    
    CDReader cdata(_cycler);
    return cdata->_angular_diameter;
}


void DirectionalLightNode::write_datagram(BamWriter* manager, Datagram& dg) {

    LightBase::write_datagram(manager, dg);
    manager->write_cdata(dg, _cycler);
}


void DirectionalLightNode::fillin(DatagramIterator& scan, BamReader* manager) {
    
    LightBase::fillin(scan, manager);
    manager->read_cdata(scan, _cycler);
}


TypedWritable* DirectionalLightNode::make_from_bam(const FactoryParams& params) {
    
    DirectionalLightNode* me = new DirectionalLightNode;
    BamReader* manager;
    DatagramIterator scan;

    parse_params(params, scan ,manager);
    me->fillin(scan, manager);

    return me;
}

DirectionalLightNode::CData::CData():
    _angular_diameter(1.0)
{
    
}


DirectionalLightNode::CData::CData(const CData& copy):
    _angular_diameter(copy._angular_diameter)
{
    
}

CycleData* DirectionalLightNode::CData::make_copy() const {
    return new CData(*this);
}

void DirectionalLightNode::CData::write_datagram(BamWriter* manager, Datagram& dg) const {

    dg.add_float32(_angular_diameter);
}

void DirectionalLightNode::CData::fillin(DatagramIterator& scan, BamReader* manager) {
    
    _angular_diameter = scan.get_float32();
}
