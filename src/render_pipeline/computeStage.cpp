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

#include <panda3d/graphicsBuffer.h>

#include "renderPipeline.h"
#include "render_utils.h"
#include "computeStage.h"

using namespace std;


DEFINE_TYPEHANDLE(ComputeStage)


void ComputeStage::update() {

    GraphicsStateGuardian* gsg = _pipeline->get_gsg();

    if (gsg->set_scene(_pipeline->get_scene_data()))
        dispatch_compute(gsg, _state, _num_groups.get_x(), _num_groups.get_y());
}


TypedWritable* ComputeStage::make_from_bam(const FactoryParams& params) {
    
    ComputeStage* me = new ComputeStage;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}


void ComputeStage::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


void ComputeStage::write_datagram(BamWriter* manager, Datagram& me) {
    
    RenderStage::write_datagram(manager, me);

    manager->write_pointer(me, _state.get_state());
    _group_size.write_datagram(me);

    me.add_uint8(_inputs.size());

    for (const Inputs::value_type& it: _inputs) {
        me.add_string(it.first);
        manager->write_pointer(me, it.second);
    }
}   


void ComputeStage::fillin(DatagramIterator& scan, BamReader* manager) {
    
    RenderStage::fillin(scan, manager);

    manager->read_pointer(scan);
    _group_size.read_datagram(scan);

    uint8_t num_inputs = scan.get_uint8();

    for (uint8_t i=0; i<num_inputs; ++i) {
        string key = scan.get_string();
        manager->read_pointer(scan);

        _inputs.insert(make_pair(key, nullptr));
    }
}


int ComputeStage::complete_pointers(TypedWritable** p_list, BamReader* manager) {
    
    int n = RenderStage::complete_pointers(p_list, manager);
    _state = DCAST(RenderState, p_list[n++]);

    for (Inputs::value_type& input: _inputs)
        input.second = DCAST(Texture, p_list[n++]);

    return n;
}


void ComputeStage::dimensions_changed(LVector2i size) {

    _num_groups.set_x(1 + ((size.get_x() - 1) / _group_size.get_x()));
    _num_groups.set_y(1 + ((size.get_y() - 1) / _group_size.get_y()));
}


void ComputeStage::setup() {

    for (const Inputs::value_type& it: _inputs)
        _state.set_shader_input(it.first, it.second);

    const SceneData* data = _pipeline->get_scene_data();

    _state.set_shader_input("u_nearFar", data->_near_far);
    _state.set_shader_input("u_viewScale", data->_view_scale);
    _state.set_shader_input("u_resolution", data->_resolution);
}

ComputeStage::ComputeStage(const string& name):
    RenderStage(name),
    _num_groups(1)
{
    
}
