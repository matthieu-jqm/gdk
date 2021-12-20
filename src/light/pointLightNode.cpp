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

#include <panda3d/boundingSphere.h>
#include <panda3d/orthographicLens.h>

#include "pointLightNode.h"


using namespace std;

DEFINE_TYPEHANDLE(PointLightNode)


PointLightNode::CData::CData(const CData &copy):
    _radius(copy._radius)
{
}

PointLightNode::CData::CData():
    _radius(1.0f)
{
}


CycleData* PointLightNode::CData::make_copy() const {
    return new CData(*this);
}


void PointLightNode::CData::fillin(DatagramIterator &scan, BamReader *manager) {
    
    _radius = scan.get_float32();
}


void PointLightNode::CData::write_datagram(BamWriter *manager, Datagram &dg) const {
    
    dg.add_float32(_radius);
}


TypedWritable* PointLightNode::make_from_bam(const FactoryParams &params) {
    
    PointLightNode* light = new PointLightNode("");
    DatagramIterator scan;
    BamReader *manager;

    parse_params(params, scan, manager);
    light->fillin(scan, manager);

    return light;
}


void PointLightNode::fillin(DatagramIterator &scan, BamReader *manager) {
    
    LightBase::fillin(scan, manager);
    manager->read_cdata(scan, _cycler);
}

void PointLightNode::write_datagram(BamWriter* manager, Datagram& dg) {

    LightBase::write_datagram(manager, dg);
    manager->write_cdata(dg, _cycler);
}


PointLightNode::PointLightNode(const string& name):
    LightBase(name, nullptr)
{
    BoundingSphere* bounds = new BoundingSphere(LPoint3(0,0,0), 1.0);

    set_internal_bounds(bounds);
    set_cull_bounds(bounds);

    Lens* lens = get_lens();

    lens->set_interocular_distance(0.0f);
    lens->set_near_far(0.01f, 1.0f);
    lens->set_fov(90.0f, 90.0f);
}


PointLightNode::PointLightNode(const PointLightNode& copy): 
    LightBase(copy),
    _cycler(copy._cycler)
{

}


PointLightNode* PointLightNode::make_copy() const {
    
    return new PointLightNode(*this);
}


int PointLightNode::get_class_priority() const {

    return (int)CP_point_priority;
}


float PointLightNode::get_radius() const {

    CDReader cdata(_cycler);
    return cdata->_radius;
}


void PointLightNode::set_radius(float radius) {

    CDWriter cdata(_cycler);
    cdata->_radius = radius;

    CPT(BoundingSphere) bound = DCAST(BoundingSphere, get_internal_bounds());
    nassertv(bound != nullptr)

    BoundingSphere* sphere_bound = const_cast<BoundingSphere*>(bound.p());
    sphere_bound->set_radius(radius);

    Lens* lens = get_lens();
    lens->set_far(radius);
}


void PointLightNode::register_with_read_factory() {

    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}



void PointLightNode::write(ostream& out, int indent_level) const {

    LightBase::write(out, indent_level);

    indent(out, indent_level + 2)
    << "radius " << get_radius() << "\n";
}