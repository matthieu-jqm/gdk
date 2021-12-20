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
#include <panda3d/displayRegionDrawCallbackData.h>
#include <panda3d/displayRegionCullCallbackData.h>

#include "multiViewShadowBuffer.h"
#include "shadowSlot.h"
#include "lightBase.h"
#include "shadowBufferManager.h"
#include "shadowBuffer.h"


using namespace std;

DEFINE_TYPEHANDLE(ShadowBuffer)



GraphicsBuffer* ShadowBuffer::make_buffer(GraphicsOutput* host) {

    GraphicsStateGuardian* gsg = host->get_gsg();
    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();


    GraphicsOutput::RenderTextureMode mode;
    FrameBufferProperties fbp;
    
    fbp.set_depth_bits(shadow_depth_bits);
    fbp.set_color_bits(0);

    GraphicsBuffer* buffer = DCAST(GraphicsBuffer,
        engine->make_output(host->get_pipe(), _info.name, -10, fbp,
                            WindowProperties::size(_resolution),
                            GraphicsPipe::BF_refuse_window | _info.flags,
                            host->get_gsg(), host));
    
    if (buffer != nullptr)
        buffer->disable_clears();
    
    return buffer;
}


void ShadowBuffer::setup(ShadowBufferManager* manager) {

    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();

    if (_buffer != nullptr)
        engine->remove_window(_buffer);

    _buffer = make_buffer(manager->get_host());

    _buffer->add_render_texture(_texture,
                    GraphicsOutput::RTM_bind_or_copy,
                    DisplayRegion::RTP_depth);

    _slots.clear();
    _slots.reserve(_num_slots);

    while (_slots.size() < _num_slots)
        _slots.resize(_slots.size() + 1, this);
}


void ShadowBuffer::update(ShadowBufferManager* manager) {

    for (Entry& entry: _entries) {

        NodePath light(entry._light);
        entry._distance = 0.0f;

        for (SceneSetup* setup: manager->_views) {
            NodePath camera = setup->get_camera_path();
            
            if (!camera.is_empty()) {
                LPoint3 pos = light.get_pos(camera);
                entry._distance += pos.length_squared();
            }
        }
    }
    std::sort(_entries.begin(), _entries.end());
    size_t num_lights = std::min(_slots.size(), _entries.size());


    for (size_t i=num_lights; i<_entries.size(); ++i)
        unbind_light(_entries[i]._light);

    for (size_t i=0; i<num_lights; ++i)
        bind_light(_entries[i]._light);
}


Texture* ShadowBuffer::get_shadow_map() const {
    return _texture;
}

GraphicsBuffer* ShadowBuffer::get_buffer() const {
    return _buffer;
}


void ShadowBuffer::register_light(LightBase* light) {

    nassertv(find_entry(light) == _entries.end())
    _entries.push_back(light);
}


void ShadowBuffer::unregister_light(LightBase* light) {

    Entries::const_iterator it = find_entry(light);
    nassertv(it != _entries.end())

    _entries.erase(it);
    unbind_light(light);
}


ShadowBuffer::Entries::iterator
ShadowBuffer::find_entry(LightBase* light) {

    Entries::iterator it = _entries.begin();

    while (it++ != _entries.end()) {
        if (it->_light == light)
            return it;
    }
    return _entries.end();
}


void ShadowBuffer::bind_light(LightBase* light) {
    
    if (light->get_shadow_slot() != -1)
        return;

    int index = _occupancy.get_lowest_off_bit();

    if (index != -1 && index < _slots.size()) {
        _slots[index]->bind(light);
        _occupancy.set_bit(index);
    }
}

void ShadowBuffer::unbind_light(LightBase* light) {
    
    int index = light->get_shadow_slot();

    if (index != -1 && index < _slots.size()) {
        _slots[index]->unbind(light);
        _occupancy.clear_bit(index);
    }
}




void ShadowBuffer::setup_texture() {
    
    _texture = new Texture(_info.name);

    SamplerState sampler;
    sampler.set_minfilter(SamplerState::FT_shadow);
    sampler.set_magfilter(SamplerState::FT_shadow);
    sampler.set_wrap_u(SamplerState::WM_border_color);
    sampler.set_wrap_v(SamplerState::WM_border_color);
    sampler.set_border_color(LColor(1.0));
    sampler.set_anisotropic_degree(0);
    sampler.set_max_lod(0);

    _texture->set_default_sampler(sampler);
    _texture->set_clear_color(LColor(1.0));
    _texture->set_keep_ram_image(false);


    _texture->setup_texture(_info.buffer_type,
                            _resolution.get_x(),
                            _resolution.get_y(),
                            _num_slots * _info.views, 
                            Texture::T_unsigned_byte,
                            Texture::F_depth_component);
}


ShadowBuffer::ShadowBuffer(const ShadowBufferInfo& info):
    _info(info)
{

    static ConfigVariableInt shadow_resolution(info.name + "-shadow-resolution",
        1024, PRC_DESC("Resolution of the shadow map"));

    static ConfigVariableInt shadow_slots(info.name + "-shadow-slots",
        16, PRC_DESC("Number of slots available for rendering shadows"));


    if (shadow_resolution.size() == 1)
        _resolution.set(shadow_resolution[0],
                        shadow_resolution[0]);

    else if (shadow_resolution.size() >= 2)
        _resolution.set(shadow_resolution[0],
                        shadow_resolution[1]);

    _num_slots = shadow_slots;

    setup_texture();
}


ShadowBuffer::~ShadowBuffer() {

    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
    engine->remove_window(_buffer);
}



bool ShadowBuffer::Entry::operator <(const Entry& other) const {
    return _distance < other._distance;
}


ShadowBuffer::Entry::Entry(LightBase* light):
    _light(light),
    _distance(0.0)
{
    
}



ShadowBuffer::Slot::Slot(ShadowBuffer* buffer):
    PointerTo<ShadowSlot>(buffer->_info.make_slot(buffer))
{
    p()->set_index(buffer->_slots.size());
}


// void ShadowBufferInfo::add_this() const {

//     _infos.ensure_up_to_date();
//     _infos.set_handler(light_type, *this, true);    
// }


// ShadowBufferInfo ShadowBufferInfo::get(TypeHandle type) {
    
//     return _infos.get_handler(type);
// }