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

#ifndef __RENDER_UTILS_H__
#define __RENDER_UTILS_H__



#include <panda3d/shaderAttrib.h>
#include <panda3d/nodePath.h>

#include "config_gdk.h"


class FrameBufferProperties;
class GraphicsBuffer;



#if defined(NDEBUG) && defined(USE_SPIRV)
    #define SHADER_LANG Shader::SL_SPIR_V

    #define VERT_SHADER_EXT "vert.spirv"
    #define GEOM_SHADER_EXT "geom.spirv"
    #define FRAG_SHADER_EXT "frag.spirv"
    #define TESS_SHADER_EXT "tess.spirv"
    #define COMP_SHADER_EXT "comp.spirv"
#else
    #define SHADER_LANG Shader::SL_GLSL

    #define VERT_SHADER_EXT "vert"
    #define GEOM_SHADER_EXT "geom"
    #define FRAG_SHADER_EXT "frag"
    #define TESS_SHADER_EXT "tess"
    #define COMP_SHADER_EXT "comp"
#endif


#define VERT_SHADER(name) name "." VERT_SHADER_EXT
#define GEOM_SHADER(name) name "." GEOM_SHADER_EXT
#define FRAG_SHADER(name) name "." FRAG_SHADER_EXT
#define TESS_SHADER(name) name "." TESS_SHADER_EXT
#define COMP_SHADER(name) name "." COMP_SHADER_EXT



std::string get_unique_name(const std::string& name, 
                            std::vector<std::string> names);


void create_2d_scene(NodePath root, NodePath& cam_np);


void create_fullscreen_quad(NodePath root, NodePath& quad_np, 
        const std::string& shader=GDK_SHADER_PATH FRAG_SHADER("texture"));


const ShaderAttrib* get_shader_attrib(NodePath node);

const Geom* get_null_geom();

void dispatch_compute(  GraphicsStateGuardian* gsg, 
                        const ShaderAttrib* attrib,
                        size_t x, size_t y=1, size_t z=1);


GraphicsBuffer* make_buffer(const FrameBufferProperties& fb_prop,
                            const std::string& name, size_t size_x, 
                            size_t size_y, int sort, GraphicsOutput* host);


void draw_fullscreen_quad(GraphicsStateGuardian* gsg,
            const RenderState* state, int num_instance=1);

void draw_geom(GraphicsStateGuardian* gsg, const Geom* geom,
            const RenderState* state, int num_instance=1);


PT(GeomNode) make_fullscreen_quad();


Texture* make_render_texture(const std::string& name);


template<class T>
CPT(T) get_render_attrib(CPT(RenderState) state) {

    return DCAST(T, state->get_attrib(T::get_class_slot()));
}


template<class ...Args>
void set_shader_input(CPT(RenderAttrib)& attr, CPT_InternalName name, Args&&... args) {

    CPT(ShaderAttrib) sattr = DCAST(ShaderAttrib, attr);

    ShaderInput input(name, std::forward<Args>(args)...);
    attr = sattr->set_shader_input(input);
}


template<class ...Args>
CPT(RenderAttrib) make_shader_input(CPT_InternalName name, Args&&... args) {

    return set_shader_input(ShaderAttrib::make(), name, std::forward<Args>(args)...);
}


template<class Data>
struct ShaderBufferArray: public std::vector<Data> {

    operator vector_uchar() const {
        using parent = std::vector<Data>;
        return vector_uchar(parent::begin(), parent::end());
    }
};


template<class Data, class ArrayData = void>
struct ShaderBufferData: public Data, public ShaderBufferArray<ArrayData> {

    operator vector_uchar() const {
        using parent = std::vector<ArrayData>;
        
        char* data = reinterpret_cast<char*>(this);

        vector_uchar buffer(data, data + sizeof(Data));
        buffer.insert(buffer.end(), parent::begin(), parent::end()); 

        return buffer;
    }
};


template<class T>
struct ShaderBufferData<T, void>: public T {

    operator vector_uchar() const {
        char* data = reinterpret_cast<char*>(this);
        return vector_uchar(data, data + sizeof(T));
    }
};

namespace glsl {

    // struct alignas(16) vec3: public LVector3f {
    struct vec3: public LVector3f {
        vec3() = default;
        vec3(const LColor& copy);
        vec3(const LVector3f& copy);
        vec3& operator =(const LVector3f& copy);
    };

    struct alignas(16) vec3i: public LVector3i {
        vec3i() = default;
        vec3i(const LVector3i& copy);
        vec3i& operator =(const LVector3i& copy);
    };

    typedef LVector4f vec4;
    typedef LVector4i vec4i;
    typedef LVector2f vec2;
    typedef LVector2i vec2i;

    typedef vec4i vec4u;
    typedef vec3i vec3u;
    typedef vec2i vec2u;
}

#endif // __RENDER_UTILS_H__