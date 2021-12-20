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

#include "renderPipeline.h"

#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/cullResult.h>
#include <panda3d/sceneSetup.h>
#include <panda3d/graphicsStateGuardian.h>
#include <panda3d/displayRegion.h>
#include <panda3d/cullBinManager.h>
#include <panda3d/graphicsEngine.h>
#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/displayRegionDrawCallbackData.h>
#include <panda3d/bamFile.h>
#include <panda3d/pStatGPUTimer.h>

#include "renderScene.h"
#include "config_render_pipeline.h"
#include "render_utils.h"
#include "renderStage.h"


using namespace std;

DEFINE_TYPEHANDLE(RenderPipeline)
DEFINE_TYPEHANDLE(RenderTarget)


RenderTarget::RenderTarget(const std::string& name):
    Texture(name),
    _scale(1.0)
{
    set_compression(Texture::CM_off);
}

void RenderTarget::set_scale(float scale) {
    _scale = scale;    
}

float RenderTarget::get_scale() const {
    return _scale;
}

void RenderTarget::update_size(LVector2i size) {

    if (_scale == 0.0f)
        return;

    float size_x = float(size.get_x()) * abs(_scale);
    float size_y = float(size.get_y()) * abs(_scale);

    if (_scale > 0.0f)
        size.set(ceil(size_x), ceil(size_y));
    else
        size.set(floor(size_x), floor(size_y));

    set_x_size(size.get_x());
    set_y_size(size.get_y());
}


void RenderTarget::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


void RenderTarget::do_write_datagram_body(CData* cdata,
                BamWriter* manager, Datagram& me)
{
    Texture::do_write_datagram_body(cdata, manager, me);
    me.add_float32(_scale);
}


void RenderTarget::do_fillin_body(CData* cdata, DatagramIterator& scan,
                BamReader* manager) 
{
    
    Texture::do_fillin_body(cdata, scan, manager);
    _scale = scan.get_float32();
}


TypedWritable* RenderTarget::make_from_bam(const FactoryParams& params) {
    
    RenderTarget* me = new RenderTarget;
    TypedWritable* object = me->make_this_from_bam(params);
    nassertr(me == object, nullptr)

    return object;
}





RenderPipeline::RenderPipeline(GraphicsOutput* host, LVecBase2i size,
                                const string& name):
    _traverser(new PipelineCullTraverser(this)),
    _initial_state(RenderState::make_empty()),
    _scene_data(new SceneData),
    _output(nullptr),
    _host(nullptr),
    _name(name)
{

    setup(host, size);
}


void RenderPipeline::setup(GraphicsOutput* host, LVecBase2i size) {

    nassertv(host != nullptr)

    if (size == LVecBase2i(0.0f) && host->has_size())
        size = host->get_size();

    _host = host;

    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();

    FrameBufferProperties fb_props;
    fb_props.clear();

    _callback_buffer = DCAST(CallbackGraphicsWindow, 
        engine->make_output(host->get_pipe(), _name, 10,
                            fb_props, WindowProperties::size(size), 
                            GraphicsPipe::BF_require_callback_window,
                            host->get_gsg(), host));


    if (_callback_buffer != nullptr)
        _callback_buffer->set_render_callback(_render_callback);

    for (RenderStage* stage: _stages) {
        stage->set_pipeline(this);
        stage->setup();
    }

    for (RenderTargets::value_type& it: _targets)
        it.second->update_size(size);
}


void RenderPipeline::add_stage(RenderStage* stage) {

    _stages.push_back(stage);

    if (_callback_buffer != nullptr) {
        stage->set_pipeline(this);
        stage->setup();
    }
}


GraphicsOutput* RenderPipeline::get_buffer() const {
    return _callback_buffer;
}

GraphicsOutput* RenderPipeline::get_host() const {
    return _host;
}

GraphicsStateGuardian* RenderPipeline::get_gsg() const {
    return _callback_buffer->get_gsg();
}

LVecBase2i RenderPipeline::get_size() const {
    return _callback_buffer->get_size();
}


void RenderPipeline::set_size(LVecBase2i size) {

    nassertv(_callback_buffer != nullptr)

    _callback_buffer->set_size_and_recalc(size.get_x(), size.get_y());

    for (RenderStage* stage: _stages)
        stage->dimensions_changed(size);

    for (RenderTargets::value_type& it: _targets)
        it.second->update_size(size);
}



void RenderPipeline::remove_stage(size_t n) {
    
    nassertv(n < _stages.size())
    Stages::iterator it = next(_stages.begin(), n);

    RenderStage* stage = *it;
    stage->release();
    stage->set_pipeline(nullptr);

    _stages.erase(it);
}


RenderStage* RenderPipeline::get_stage(size_t n) const {
    
    nassertr(n < _stages.size(), nullptr)
    return *next(_stages.begin(), n);
}


size_t RenderPipeline::get_num_stage() const {

    return _stages.size();
}


void RenderPipeline::hook_display_regions() {

    int num_region = _callback_buffer->get_num_active_display_regions();

    for (int i=0; i<num_region; ++i) {
        PT(DisplayRegion) region = _callback_buffer->get_active_display_region(i);

        if (UNLIKELY(region->get_cull_traverser() != _traverser))
            region->set_cull_traverser(_traverser);     
    }
}


Texture* RenderPipeline::get_output() const {

    return _output;
}


void RenderPipeline::set_tag_name(const string& index) {
    _tag_name = index;    
}

std::string RenderPipeline::get_tag_name() const {
    return _tag_name;
}


SceneData* RenderPipeline::get_scene_data() const {

    return _scene_data;    
}


void RenderPipeline::register_node_handler(NodeHandler* handler, bool subtypes) {

    TypeHandle type = handler->get_node_type();

    _node_handlers.set_handler(type, handler, subtypes);
    _node_handlers.insert(type);

    handler->setup(this);  
}


void RenderPipeline::unregister_node_handler(NodeHandler* handler, bool subtypes) {
    
    TypeHandle type = handler->get_node_type();

    _node_handlers.set_handler(type, nullptr, subtypes);
    _node_handlers.erase(type);

    handler->cleanup(this);
}


NodeHandler* RenderPipeline::get_handler(TypeHandle type) const {
    
    return _node_handlers.get_handler(type);
}


RenderTarget* RenderPipeline::get_render_target(const std::string& name) {
    
    RenderTargets::const_iterator it = _targets.find(name);

    if (it != _targets.end())
        return it->second;

    return nullptr;
}


void RenderPipeline::garbage_collect() {

    RenderTargets::iterator it = _targets.begin();

    while (it != _targets.end()) {
        RenderTarget* target = it->second;

        if (target->get_ref_count() == 1)
            _targets.erase(it++);
        else
            ++it;
    }    
}


void RenderPipeline::register_with_read_factory() {

    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);    
}


PStatCollector& RenderPipeline::get_pcollector() {

    if (_callback_buffer != nullptr)
        return _callback_buffer->get_draw_window_pcollector();

    static PStatCollector default_collector;
    return default_collector;    
}


RenderPipeline::RenderPipeline():
    _traverser(new PipelineCullTraverser(this)),
    _initial_state(RenderState::make_empty()),
    _scene_data(new SceneData),
    _name("render-pipeline"),
    _callback_buffer(nullptr),
    _host(nullptr),
    _output(nullptr)
{
    
}



void RenderPipeline::render(RenderCallbackData* data) {
    
    using RenderCallbackType = CallbackGraphicsWindow::RenderCallbackType;

    data->set_render_flag(false);

    if (data->get_callback_type() != RenderCallbackType::RCT_begin_frame
            || data->get_frame_mode() != GraphicsOutput::FM_render)
        return;

    if (_host->flip_ready()) {
        _host->begin_flip();
        _host->end_flip();
    }
    GraphicsStateGuardian* gsg = get_gsg();

    gsg->push_group_marker("render-pipeline:" + _name);

    // now render the stages
    for (RenderStage* stage: _stages) {

        if (!stage->is_active())
            continue;
        
        PStatGPUTimer timer(gsg, stage->get_pcollector());

        gsg->push_group_marker(stage->get_name());
        stage->update();
        gsg->pop_group_marker();
    }

    gsg->pop_group_marker();

    hook_display_regions();
    garbage_collect();
}


void RenderPipeline::write_datagram(BamWriter* manager, Datagram& dg) {

    dg.add_string(_name);
    
    manager->write_pointer(dg, _initial_state);
    manager->write_pointer(dg, _output);

    dg.add_uint16(_stages.size());
    for (RenderStage* stage: _stages)
        manager->write_pointer(dg, stage);

    dg.add_uint16(_targets.size());
    for (RenderTargets::value_type& it: _targets)
        manager->write_pointer(dg, it.second);
}


void RenderPipeline::fillin(DatagramIterator& scan, BamReader* manager) {
    
    _name = scan.get_string();

    manager->read_pointers(scan, 2);

    _stages.resize(scan.get_uint16());
    manager->read_pointers(scan, _stages.size());

    _num_targets = scan.get_uint16();
    manager->read_pointers(scan, _num_targets);
}


int RenderPipeline::complete_pointers(TypedWritable** p_list, BamReader* manager) {

    int n = 0;

    _initial_state = DCAST(RenderState, p_list[n++]);
    _output = DCAST(RenderTarget, p_list[n++]);

    for (Stages::iterator it = _stages.begin(); it != _stages.end(); ++it)
        (*it) = DCAST(RenderStage, p_list[n++]);

    for (uint16_t i=0; i<_num_targets; ++i) {
        RenderTarget* target = DCAST(RenderTarget, p_list[n++]);
        _targets.insert(make_pair(target->get_name(), target));
    }
    return n;
}


TypedWritable* RenderPipeline::make_from_bam(const FactoryParams& params) {

    RenderPipeline* me = new RenderPipeline;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}


const RenderState* RenderPipeline::get_initial_state() const {

    return _initial_state;
}


void RenderPipeline::set_initilal_state(const RenderState* state) {

    _initial_state = state;
}


RenderPipeline::~RenderPipeline() {

    for (RenderStage* stage: _stages) {
        stage->release();
        stage->set_pipeline(nullptr);
    }

    if (_callback_buffer != nullptr) {
        GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
        engine->remove_window(_callback_buffer);

        int num_dr = _callback_buffer->get_num_display_regions();

        for (int i=0; i<num_dr; ++i) {
            PT(DisplayRegion) dr = _callback_buffer->get_display_region(i);

            if (dr->get_cull_traverser() == _traverser)
                dr->set_cull_traverser(new CullTraverser);
        }
        nassertv(_traverser->get_ref_count() == 1)
    }
}


// void RenderPipeline::cull(DisplayRegionCullCallbackData* data) {
    
//     Thread* current_thread = Thread::get_current_thread();

//     SceneSetup* setup = data->get_scene_setup();
//     DisplayRegion* dr = setup->get_display_region();
//     GraphicsStateGuardian* gsg = _callback_buffer->get_gsg();


//     if (LIKELY(dr->get_window() == _callback_buffer)) {

//         if (dr == _callback_buffer->get_active_display_region(0))
//             _scene_data->update_data(setup);

//         CPT(RenderState) state = setup->get_initial_state();
//         setup->set_initial_state(_initial_state->compose(state));


//         CullHandlerHook hook(data->get_cull_handler(), this);
//         GraphicsEngine::do_cull(&hook, setup, gsg, current_thread);

//         for (NodeHandlers::value_type it: _node_handlers)
//             it.second->post_cull(dr->get_cull_traverser());
//     } 
//     else {
//         dr->clear_cull_callback();
//         data->upcall();
//     }
// }

