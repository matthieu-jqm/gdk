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

#include <panda3d/cullTraverser.h>
#include <panda3d/cullableObject.h>
#include <panda3d/cullHandler.h>

#include "shadowBufferManager.h"
#include "cullHandlerHook.h"
#include "lightBase.h"


using namespace std;

DEFINE_TYPEHANDLE(LightBase)


float LightBase::get_energy() const {

    CDReader cdata(_cycler);
    return cdata->_energy;
}


void LightBase::set_energy(float energy) {

    nassertv(energy >= 0.0f)

    CDWriter cdata(_cycler);
    cdata->_energy = energy;
}


LightBase::LightBase(const string& name, Lens* lens):
    LightLensNode(name, lens)
{
    set_color(LColor(1.0f));

    // DrawMask mask = get_camera_mask();
    // mask.clear_bit(Map::RL_transparent);

    // set_camera_mask(mask);
}


LightBase::LightBase(const LightBase& copy): 
    LightLensNode(copy),
    _cycler(copy._cycler)
{

}


void LightBase::add_for_draw(CullTraverser* trav, CullTraverserData& data) {

    if (trav->is_of_type(PipelineCullTraverser::get_class_type())) {
        PipelineCullTraverser* traverser = (PipelineCullTraverser*)trav;

        traverser->record_node(data);
    }
}



bool LightBase::is_renderable() const {    
    return true;
}

bool LightBase::safe_to_combine() const  {
    return false;
}

bool LightBase::safe_to_flatten() const {
    return false;
}


void LightBase::setup_shadow_map() {
    
}


void LightBase::fillin(DatagramIterator &scan, BamReader *manager) {

    LightLensNode::fillin(scan, manager);
    manager->read_cdata(scan, _cycler);
}


void LightBase::write_datagram(BamWriter* manager, Datagram& dg) {
    
    LightLensNode::write_datagram(manager, dg);
    manager->write_cdata(dg, _cycler);
}


LightBase::CData::CData(): 
    _energy(1.0f), 
    _shadow_slot(-1)
{
    
}


LightBase::CData::CData(const CData& copy): 
    _energy(copy._energy),
    _shadow_slot(-1)
{
    
}


CycleData* LightBase::CData::make_copy() const {
    return new CData(*this);
}


void LightBase::CData::write_datagram(BamWriter *manager, Datagram &dg) const {

    dg.add_float32(_energy);
}


void LightBase::CData::fillin(DatagramIterator &scan, BamReader *manager) {
    
    _energy = scan.get_float32();
}


void LightBase::bind(GraphicsStateGuardianBase* gsg, 
                    const NodePath& light, int light_id) 
{
}

void LightBase::set_shadow_caster(bool caster) {
    
    CDWriter cdata(_cycler);

    if (UNLIKELY(caster == _shadow_caster))
        return;
    
    ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();
    
    _shadow_caster = caster;
    set_active(caster);

    if (caster)
        mgr->register_light(this);
    else
        mgr->unregister_light(this);
}


int LightBase::get_shadow_slot() const {
    
    CDReader cdata(_cycler);
    return cdata->_shadow_slot;
}


void LightBase::set_shadow_slot(int slot) {

    CDWriter cdata(_cycler);
    cdata->_shadow_slot = slot;
}


LightBase::~LightBase() {

    if (_shadow_caster) {
        ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();
        mgr->unregister_light(this);
    }    
}
