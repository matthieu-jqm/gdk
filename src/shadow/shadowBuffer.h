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

#ifndef __SHADOWBUFFER_H__
#define __SHADOWBUFFER_H__

#include <panda3d/graphicsBuffer.h>

#include "shaderState.h"
#include "graphicsCallback.h"
#include "typeHandler.h"
#include "utils.h"

class GraphicsBuffer;
class ShadowBufferManager;
class LightBase;
class ShadowSlot;
class ShadowBuffer;


struct ShadowBufferInfo {

    std::string name;
    TypeHandle light_type;

    Texture::TextureType buffer_type;
    int views;
    int flags;

    ShadowSlot* (*make_slot)(ShadowBuffer*);

//     void add_this() const;
//     static ShadowBufferInfo get(TypeHandle type);

// private:
//     static TypeHandler<ShadowBufferInfo> _infos;
};



class ShadowBuffer: public TypedReferenceCount {

    REGISTER_TYPE("ShadowBuffer", TypedReferenceCount)

public:
    ShadowBuffer(const ShadowBufferInfo& info);

    virtual ~ShadowBuffer();

    const ShadowBufferInfo _info;

    void register_light(LightBase* light);
    void unregister_light(LightBase* light);

    void unbind_light(LightBase* light);
    void bind_light(LightBase* light);

    virtual void setup(ShadowBufferManager* manager);
    virtual void update(ShadowBufferManager* manager);

    Texture* get_shadow_map() const;
    GraphicsBuffer* get_buffer() const;

protected:
    class Entry {
    public:
        Entry(LightBase* light=nullptr);
        ~Entry() = default;

        bool operator < (const Entry& other) const;

        LightBase* _light;
        float _distance;
    };

    class Slot: public PointerTo<ShadowSlot> {
    public:
        Slot(ShadowBuffer* buffer);
        ~Slot() = default;
    };

    typedef std::vector<Entry> Entries;
    typedef std::vector<Slot> Slots;

    Entries _entries;
    Slots _slots;

    BitArray _occupancy;
    LVector2i _resolution;
    size_t _num_slots;

    PT(GraphicsBuffer) _buffer;
    PT(Texture) _texture;

    GraphicsBuffer* make_buffer(GraphicsOutput* host);
    Entries::iterator find_entry(LightBase* light);

private:
    void setup_texture();
};


#endif // __SHADOWBUFFER_H__