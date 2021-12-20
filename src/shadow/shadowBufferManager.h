

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


#ifndef __SHADOWBUFFERMANAGER_H__
#define __SHADOWBUFFERMANAGER_H__

#include "shadowBuffer.h"
#include "callbacks.h"
#include "typeHandler.h"

class ShadowBuffer;
class GraphicsOutput;
class Texture;
class LightBase;


// struct ALIGN_16BYTE ShadowMap {

//     uint32_t _layer;
//     float _opacity;
// };


class ShadowBufferManager: public TaskPool {

public:
    ~ShadowBufferManager() = default;

    static ShadowBufferManager* get_global_ptr();

    void register_light(LightBase* light);
    void unregister_light(LightBase* light);

    void set_tag_name(const std::string& name);

    GraphicsOutput* get_host() const;

    ShadowBuffer* get_buffer(TypeHandle type) const;
    void add_buffer(ShadowBufferInfo info);

    typedef std::vector<PT(SceneSetup)> Views;
    Views _views;

private:
    ShadowBufferManager();
    
    void determine_effective_views();
    void add_view(DisplayRegion* region);

    typedef std::map<TypeHandle, PT(ShadowBuffer)> Buffers;
    Buffers _buffers;

    mutable PT(GraphicsOutput) _host;
    std::string _tag_name;
};


#endif // __SHADOWBUFFERMANAGER_H__







    // ShadowSize _shadow_map_size;
    // ShadowSize _shadow_cubemap_size;


    // void set_shadow_map_size(ShadowSize size);
    // void set_shadow_cubemap_size(ShadowSize size);
    
    // ShadowSize get_shadow_map_size() const;
    // ShadowSize get_shadow_cubemap_size() const;
