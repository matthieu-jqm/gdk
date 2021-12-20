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

#include "shadowSlot.h"
#include "lightBase.h"
#include "shadowBufferManager.h"
#include "multiViewShadowBuffer.h"

using namespace std;

DEFINE_TYPEHANDLE(MultiViewShadowBuffer)


MultiViewShadowBuffer::MultiViewShadowBuffer(const ShadowBufferInfo& info):
    ShadowBuffer(info)
{

}


MultiViewShadowBuffer::~MultiViewShadowBuffer() {
    
    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();
    engine->remove_window(_buffer);
}


GraphicsBuffer* MultiViewShadowBuffer::get_buffer() const {
    return _buffer;
}


void MultiViewShadowBuffer::setup(ShadowBufferManager* manager) {

    _buffer = make_buffer(manager->get_host());

    _buffer->add_render_texture(_texture,
                    GraphicsOutput::RTM_bind_layered,
                    DisplayRegion::RTP_depth);

    _buffer->set_clear_depth_active(true);

    ShadowBuffer::setup(manager);

    // we need to render the parent buffer first as it must clear the depth texture
    ShadowBuffer::_buffer->set_sort(_buffer->get_sort() - 1);
    ShadowBuffer::_buffer->set_active(false);
    const_cast<string&>(ShadowBuffer::_buffer->get_name()) = "clear_" + _info.name;
}

void MultiViewShadowBuffer::update(ShadowBufferManager* manager) {
    
    ShadowBuffer::update(manager);
}