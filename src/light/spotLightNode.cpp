#include "spotLightNode.h"

using namespace std;

DEFINE_TYPEHANDLE(SpotLightNode)


SpotLightNode::SpotLightNode(const string& name): 
    LightBase(name, new PerspectiveLens())
{
    
}


SpotLightNode::SpotLightNode(const SpotLightNode& copy):
        LightBase(copy),
        _cycler(copy._cycler)
{
    
}

float SpotLightNode::get_max_distance() const {
    
    CDReader cdata(_cycler);
    return cdata->_max_distance;
}

void SpotLightNode::set_max_distance(float distance) {
    
    CDWriter cdata(_cycler);
    cdata->_max_distance = distance;
}

float SpotLightNode::get_inner_radius() const {
    
    CDReader cdata(_cycler);
    return cdata->_inner_radius;
}

void SpotLightNode::set_inner_radius(float radius) {
    
    CDWriter cdata(_cycler);
    cdata->_inner_radius = radius;
}

float SpotLightNode::get_outter_radius() const {
    
    CDReader cdata(_cycler);
    return cdata->_outter_radius;
}

void SpotLightNode::set_outter_radius(float radius) {
    
    CDWriter cdata(_cycler);
    cdata->_outter_radius = radius;
}

void SpotLightNode::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


int SpotLightNode::get_class_priority() const {
    return CP_spot_priority;
}


void SpotLightNode::write_datagram(BamWriter* manager, Datagram& dg) {
    
    LightBase::write_datagram(manager, dg);
    manager->write_cdata(dg, _cycler);
}

void SpotLightNode::fillin(DatagramIterator& scan, BamReader* manager) {

    LightBase::fillin(scan, manager);
    manager->read_cdata(scan, _cycler);
}


TypedWritable* SpotLightNode::make_from_bam(const FactoryParams& params) {
    
    SpotLightNode* light = new SpotLightNode("");
    DatagramIterator scan;
    BamReader *manager;

    parse_params(params, scan, manager);
    light->fillin(scan, manager);

    return light;
}


SpotLightNode::CData::CData():
    _max_distance(10.0f),
    _inner_radius(0.2f),
    _outter_radius(0.5f)
{
    
}


SpotLightNode::CData::CData(const CData& copy):
    _max_distance(copy._max_distance),
    _inner_radius(copy._inner_radius),
    _outter_radius(copy._outter_radius)
{
    
}

CycleData* SpotLightNode::CData::make_copy() const {
    return new CData(*this);
}


void SpotLightNode::CData::write_datagram(BamWriter* manager, Datagram& dg) const {

    dg.add_float32(_max_distance);
    dg.add_float32(_inner_radius);
    dg.add_float32(_outter_radius);
}


void SpotLightNode::CData::fillin(DatagramIterator& scan, BamReader* manager) {
    
    _max_distance = scan.get_float32();
    _inner_radius = scan.get_float32();
    _outter_radius = scan.get_float32();
}