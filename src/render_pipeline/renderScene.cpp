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

#include <panda3d/graphicsEngine.h>
#include <panda3d/graphicsEngine.h>
#include <panda3d/pStatGPUTimer.h>

#include "config_render_pipeline.h"
#include "renderPipeline.h"
#include "render_utils.h"
#include "renderScene.h"

using namespace std;

DEFINE_TYPEHANDLE(RenderScene)


using BinType = CullBinEnums::BinType;


bool RenderScene::setup_buffer(LVector2i size) {

    nassertr(_pipeline != nullptr, false)

    _buffer = make_buffer(_fb_props, get_name(),
                        size.get_x(), size.get_y(),
                        0, _pipeline->get_host());
    
    if (UNLIKELY(_buffer == nullptr))
        return false;
    
    // We have to make sure the GraphicsOutput is disabled so that it is not
    // rendered by the GraphicsEngine as we take care of rendering it.
    _buffer->set_active(false);
    _buffer->disable_clears();

    for (int i=0; i<_bindings.size(); ++i) {

        RenderTexturePlane plane = (RenderTexturePlane)i;
        const BindingSlot& slot = _bindings[i];

        if (slot._target == nullptr)
            continue;

        _buffer->add_render_texture(slot._target, 
                GraphicsOutput::RTM_bind_or_copy, plane);
        
        if (plane == RenderTexturePlane::RTP_depth_stencil) {
            _buffer->set_clear_depth_active(slot._clear);
            _buffer->set_clear_stencil_active(slot._clear);
        } else
            _buffer->set_clear_active(plane, slot._clear);
    }

    return true;
}


void RenderScene::dimensions_changed(LVector2i size) {

    nassertv(_buffer != nullptr)
    _buffer->set_size(size.get_x(), size.get_y());
}


void RenderScene::add_bin(const string& name, BinType type, int sort) {

    CullBinManager* mgr = CullBinManager::get_global_ptr();
    int bin_index = mgr->find_bin(name);

    if (bin_index == -1)
        bin_index = mgr->add_bin(name, type, sort);

    _bin_indices.push_back(bin_index);
}


void RenderScene::remove_bin(const string& name) {
    
    CullBinManager* mgr = CullBinManager::get_global_ptr();
    int bin_index = mgr->find_bin(name);

    if (bin_index != -1) {
        auto it = find(_bin_indices.begin(), _bin_indices.end(), bin_index);

        if (it != _bin_indices.end())
            _bin_indices.erase(it);
    }
}


void RenderScene::update() {

    nassertv(_buffer != nullptr)

    Thread* current_thread = Thread::get_current_thread();
    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
    GraphicsStateGuardian* gsg = _buffer->get_gsg();

    if (_buffer->begin_frame(GraphicsOutput::FM_render, current_thread)) {

        if (_buffer->is_any_clear_active()) {
            gsg->push_group_marker("Clear");
            _buffer->clear(current_thread);
            gsg->pop_group_marker();
        }
        render(current_thread);

        _buffer->end_frame(GraphicsOutput::FM_render, current_thread);
    }
}


void RenderScene::render(Thread* current_thread) {

    GraphicsOutput* output = _pipeline->get_buffer();
    int num_dr = output->get_num_active_display_regions();

    for (int i = 0; i < num_dr; ++i)
        draw(output->get_active_display_region(i), current_thread);
}


void RenderScene::draw(DisplayRegion* dr, Thread* current_thread) {

    GraphicsStateGuardian* gsg = _buffer->get_gsg();
    SceneSetup* setup = dr->get_scene_setup(current_thread);
    CullResult* result = dr->get_cull_result(current_thread);

    if (setup == nullptr || result == nullptr)
        return;

    bool force = !gsg->get_effective_incomplete_render();
    gsg->clear_state_and_transform();

    DisplayRegionPipelineReader dr_reader(dr, current_thread);
    _buffer->change_scenes(&dr_reader);

    dr->compute_pixels(_buffer->get_fb_x_size(), _buffer->get_fb_y_size());
    gsg->prepare_display_region(&dr_reader);

    if (dr_reader.is_any_clear_active()) {
        PStatGPUTimer timer(gsg, _buffer->get_clear_window_pcollector(), 
                                current_thread);
        gsg->clear(dr_reader.get_object());
    }

    if (!gsg->set_scene(setup))
        display_cat.error() << gsg->get_type() 
            << "cannot set scene setup" << endl;

    else if (gsg->begin_scene()) {
        
        for (int bin_index : _bin_indices) {
            CullBin* bin = result->get_bin(bin_index);

            gsg->push_group_marker(bin->get_name());
            bin->draw(force, current_thread);
            gsg->pop_group_marker();
        }
        gsg->end_scene();
    }

    dr->compute_pixels();
}


void RenderScene::add_target(RenderTexturePlane binding) {

    switch (binding) {

        case RenderTexturePlane::RTP_aux_rgba_0:
        case RenderTexturePlane::RTP_aux_rgba_1:
        case RenderTexturePlane::RTP_aux_rgba_2:
        case RenderTexturePlane::RTP_aux_rgba_3:
            _fb_props.set_aux_rgba(_fb_props.get_aux_rgba() + 1);
            break;
        case RenderTexturePlane::RTP_aux_hrgba_0:
        case RenderTexturePlane::RTP_aux_hrgba_1:
        case RenderTexturePlane::RTP_aux_hrgba_2:
        case RenderTexturePlane::RTP_aux_hrgba_3:
            _fb_props.set_aux_hrgba(_fb_props.get_aux_hrgba() + 1);
            break;
        case RenderTexturePlane::RTP_aux_float_0:
        case RenderTexturePlane::RTP_aux_float_1:
        case RenderTexturePlane::RTP_aux_float_2:
        case RenderTexturePlane::RTP_aux_float_3:
            _fb_props.set_aux_float(_fb_props.get_aux_float() + 1);
            break;
        case RenderTexturePlane::RTP_depth:
            _fb_props.set_depth_bits(24);
            break;
        case RenderTexturePlane::RTP_stencil:
            _fb_props.set_stencil_bits(8);
            break;
        case RenderTexturePlane::RTP_depth_stencil:
            _fb_props.set_depth_bits(24);
            _fb_props.set_stencil_bits(8);
            break;
        default:
            break;
    }
}


void RenderScene::register_with_read_factory() {

    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);    
}


void RenderScene::write_datagram(BamWriter* manager, Datagram& me) {

    RenderStage::write_datagram(manager, me);
    
    me.add_uint8(_fb_props.get_multisamples());
    me.add_uint8(_fb_props.get_red_bits());
    me.add_uint8(_fb_props.get_green_bits());
    me.add_uint8(_fb_props.get_blue_bits());
    me.add_uint8(_fb_props.get_alpha_bits());
    me.add_uint8(_fb_props.get_accum_bits());
    me.add_uint8(_fb_props.get_back_buffers());


    CullBinManager* mgr = CullBinManager::get_global_ptr();
    me.add_uint8(_bin_indices.size());

    for (int bin_index : _bin_indices) {
  
        me.add_string(mgr->get_bin_name(bin_index));
        me.add_uint8(mgr->get_bin_type(bin_index));
        me.add_int32(mgr->get_bin_sort(bin_index));
    }

    for (const BindingSlot& slot : _bindings) {
        me.add_bool(slot._clear);
        manager->write_pointer(me, slot._target);
    }
}


void RenderScene::fillin(DatagramIterator& scan, BamReader *manager) {
    
    RenderStage::fillin(scan, manager);   

    _fb_props.set_multisamples(scan.get_uint8());
    _fb_props.set_red_bits(scan.get_uint8());
    _fb_props.set_green_bits(scan.get_uint8());
    _fb_props.set_blue_bits(scan.get_uint8());
    _fb_props.set_alpha_bits(scan.get_uint8());
    _fb_props.set_accum_bits(scan.get_uint8());
    _fb_props.set_back_buffers(scan.get_uint8());


    uint8_t num_bins = scan.get_uint8();

    for (uint8_t i=0; i<num_bins; ++i) {

        string name = scan.get_string();
        BinType type = (BinType)scan.get_uint8();
        int32_t sort = scan.get_int32();

        add_bin(name, type, sort);
    }


    for (BindingSlot& slot: _bindings) {
        slot._clear = scan.get_bool();
        manager->read_pointer(scan);
    }
}

int RenderScene::complete_pointers(TypedWritable** p_list, BamReader* reader) {
    
    int n = RenderStage::complete_pointers(p_list, reader);

    for (int i=0; i<_bindings.size(); ++i) {
        BindingSlot& slot = _bindings[i];
        slot._target = DCAST(Texture, p_list[n++]);

        if (slot._target != nullptr)
            add_target((RenderTexturePlane)i);
    }
    return n;
}


TypedWritable* RenderScene::make_from_bam(const FactoryParams& params) {
    
    RenderScene* me = new RenderScene;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}


void RenderScene::release() {
    
    if (_buffer != nullptr) {
        GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
        engine->remove_window(_buffer);

        if (_buffer->get_ref_count() != 1)
            rp_warning("buffer " << get_name() << " is still used");

        _buffer = nullptr;
    }
}


void RenderScene::setup() {

    setup_buffer(_pipeline->get_size());
}


RenderScene::RenderScene(const string& name):
    RenderStage(name)
{
    _fb_props.set_force_hardware(true);
    _fb_props.set_stereo(false);
    _fb_props.set_rgba_bits(8,8,8,8);
    _fb_props.set_multisamples(0);
    _fb_props.set_accum_bits(0);
    _fb_props.set_back_buffers(0);
}
