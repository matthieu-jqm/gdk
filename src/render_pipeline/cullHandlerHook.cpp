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

#include "cullHandlerHook.h"
#include "renderPipeline.h"


using namespace std;


DEFINE_TYPEHANDLE(NodeHandler)
DEFINE_TYPEHANDLE(PipelineCullTraverser)



void NodeHandler::post_cull(const PipelineCullTraverser* trav) {
    
}

void NodeHandler::setup(RenderPipeline* pipeline) {
    
}

void NodeHandler::cleanup(RenderPipeline* pipeline) {
    
}


PipelineCullTraverser::PipelineCullTraverser(RenderPipeline* pipeline):
    _pipeline(pipeline)
{
    
}


void PipelineCullTraverser::set_scene(SceneSetup* scene_setup,
                         GraphicsStateGuardianBase* gsg,
                         bool dr_incomplete_render)
{

    Camera* camera = scene_setup->get_camera_node();
    DisplayRegion* dr = scene_setup->get_display_region();
    
    const RenderState* state = scene_setup->get_initial_state();
    const RenderState* pipeline_state = _pipeline->get_initial_state();

    scene_setup->set_initial_state(state->compose(pipeline_state));

    string tag_name = _pipeline->_tag_name;

    if (camera != nullptr && !tag_name.empty()) {
        if (camera->get_tag_state_key() == tag_name)
            _pipeline->_scene_data->update_data(scene_setup);
    }

    CullTraverser::set_scene(scene_setup, gsg, dr_incomplete_render);
}


void PipelineCullTraverser::end_traverse() {

    CullTraverser::end_traverse();

    for (TypeHandle type: _pipeline->_node_handlers) {
        NodeHandler* handler = _pipeline->get_handler(type);
        
        handler->post_cull(this);
    }
}


void PipelineCullTraverser::record_node(CullTraverserData& data) {
    
    PandaNode* node = data.node();
    TypeHandle type = node->get_type();

    NodeHandler* handler = _pipeline->get_handler(type);

    if (LIKELY(handler != nullptr))
        handler->record_node(data, this);
}


RenderPipeline* PipelineCullTraverser::get_pipeline() const {
    return _pipeline;
}


// DEFINE_ATTRIB(NodeInfoAttrib)


// CullHandlerHook::CullHandlerHook(CullHandler* actual_handler,
//                                 RenderPipeline* pipeline):
//     _actual_handler(actual_handler),
//     _pipeline(pipeline)
// {
    
// }


// void CullHandlerHook::record_object(CullableObject* object, const CullTraverser* trav) {

//     const NodeInfoAttrib* attrib;

//     if (object->_state->get_attrib(attrib) &&
//         !_pipeline->record_node(object, attrib, trav))

//         delete object;
//     else
//         _actual_handler->record_object(object, trav);
// }


// NodeInfoAttrib::NodeInfoAttrib(PandaNode* node):
//     _node(node)
// {

// }


// bool NodeInfoAttrib::lower_attrib_can_override() const {
//     return true;
// }

// CPT(RenderAttrib) NodeInfoAttrib::make(PandaNode* node) {

//    return return_new(new NodeInfoAttrib(node));
// }


// void NodeInfoAttrib::init_slot() {
    
//     _attrib_slot = register_slot(_type_handle, 100, 
//         new NodeInfoAttrib(nullptr));
// }

