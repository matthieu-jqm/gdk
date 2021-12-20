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

#include <panda3d/cullTraverserData.h>
#include <panda3d/cullHandler.h>
#include <panda3d/graphicsStateGuardian.h>
#include <panda3d/glgsg.h>
#include <panda3d/boundingHexahedron.h>
#include <panda3d/frustum.h>

#include "pointLightNode.h"
#include "spotLightNode.h"
#include "directionalLightNode.h"

#include "lighting.h"
#include "config_render_pipeline.h"
#include "lightBase.h"
#include "lightPool.h"

using namespace std;


DEFINE_TYPEHANDLE(LightPoolBase)

#define MAX_SHADOW_SLOT 32


const size_t shadows_size = MAX_SHADOW_SLOT * sizeof(ShadowData);


LightPoolBase::PoolConstructors LightPoolBase::_contructors;



LightPoolBase* LightPoolBase::make(LightType type) {

    PoolConstructor* constructor = _contructors[type];
    
    if (constructor == nullptr)
        return nullptr;
    
    return constructor();
}


LightPoolBase::LightPoolBase(const string& name, size_t data_size):
    Namable(name),
    _stage(nullptr),
    _data_size(data_size),
    _min_buffer_size(shadows_size + data_size)
{

    CDWriter cdata(_cycler);
    cdata->_data.resize(_min_buffer_size);
}


void LightPoolBase::set_data(void* data) {

    CDWriter cdata(_cycler);

    size_t offset = cdata->_num_lights * _data_size;
    vector_uchar::iterator it = prev(cdata->_data.end(), offset);

    uchar* start = (uchar*)data;
    uchar* end = start + _data_size;

    cdata->_data.insert(it, start, end);
}


void LightPoolBase::set_data_size(size_t size) {
    
}


void LightPoolBase::record_node(CullTraverserData& data,
                            const PipelineCullTraverser* trav)
{
    CDWriter cdata(_cycler);

    static const LMatrix4 biasmat(0.5f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 0.5f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 0.5f, 0.0f,
                                  0.5f, 0.5f, 0.5f, 1.0f);

    LightBase* light = DCAST(LightBase, data.node());
    ++cdata->_num_lights;

    int shadow_slot = light->get_shadow_slot();

    if (shadow_slot != -1) {

        ShadowData* buffer = (ShadowData*)cdata->_data.data();
        ShadowData& shadow_data = buffer[shadow_slot];

        Lens* lens = light->get_lens();

        CPT(TransformState) tr = data.get_internal_transform(trav);
        LMatrix4 mat = tr->get_inverse()->get_mat();
        LVector4 user_data;

        if (light->is_of_type(PointLightNode::get_class_type())) {
            user_data.set_x(lens->get_near());
            user_data.set_y(lens->get_far());
        } else
            mat *= lens->get_projection_mat() * biasmat;
        
        shadow_data.transform = mat;
        shadow_data.data = user_data;
    }
}


void LightPoolBase::post_cull(const PipelineCullTraverser* trav) {
    
    CDWriter cdata(_cycler);

    size_t data_size = shadows_size + cdata->_num_lights * _data_size;
    cdata->_data.resize(max(_min_buffer_size, data_size));

    commit_data(cdata->_data.data() + shadows_size);
}


void LightPoolBase::commit_data(uchar* data) {
    
}


LightPoolBase::CData::CData():
    _num_lights(0)
{
    
}


LightPoolBase::CData::CData(const CData& copy):
    _data(copy._data),
    _num_lights(copy._num_lights)
{
    
}


CycleData* LightPoolBase::CData::make_copy() const {
    return new CData(*this);
}


void LightPoolBase::set_stage(Lighting* stage) {
    _stage = stage;
}


void LightPoolBase::collect_data(vector_uchar& data, int& n) {

    CDWriter cdata(_cycler);
    
    data.resize(_min_buffer_size), n = 0;

    swap(cdata->_data, data);
    swap(cdata->_num_lights, n);
}


























   // static PT(CallbackObject) callback = nullptr;
    // if (UNLIKELY(callback == nullptr))
    //     callback = new DrawLightCallback;

// DEFINE_TYPEHANDLE(DrawLightCallback)


// void DrawLightCallback::do_callback(CallbackData* cdata) {

//     GeomDrawCallbackData* data = DCAST(GeomDrawCallbackData, cdata);

//     data->set_lost_state(false);

//     GraphicsStateGuardianBase* gsg = data->get_gsg();

//     if (LIKELY(gsg->is_of_type(GLGraphicsStateGuardian::get_class_type()))) {
//         GLGraphicsStateGuardian* glgsg = (GLGraphicsStateGuardian*)gsg;
//         glgsg->issue_memory_barrier(GL_ALL_BARRIER_BITS);
//     }

//     data->upcall();
// }
