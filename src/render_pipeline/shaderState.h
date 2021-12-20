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


#ifndef __SHADERSTATE_H__
#define __SHADERSTATE_H__

#include <panda3d/nodePath.h>
#include <panda3d/cullFaceAttrib.h>


class ShaderState {

public:
    INLINE ShaderState();
    INLINE ShaderState(const ShaderState& copy);
    INLINE ShaderState(const RenderState* state);
    ~ShaderState() = default;

    INLINE CPT(RenderState) make_copy() const;

    INLINE void clear_shader();
    INLINE bool has_shader() const;
    INLINE void set_shader(Shader* shader, int priority=0);
    INLINE const Shader* get_shader() const;

    INLINE void set_material_off(int priority=0);
    INLINE void set_light_off(int priority=0);

    INLINE void set_flag(int flag, bool value);
    INLINE bool get_flag(int flag) const;

    INLINE void set_color(LColor color, int priority=0);
    INLINE LColor get_color() const;

    INLINE void compose(const RenderState* other);

    INLINE void set_bin(const std::string& name, int draw_order, int priority=0);
    INLINE void set_depth_write(bool enable, int priority=0);
    INLINE void set_depth_test(bool enable, int priority=0);
    INLINE void set_two_sided(bool enable, int priority=0);

    INLINE void set_shader_input(const ShaderInput& input);
    INLINE void set_shader_input(ShaderInput&& input);

    INLINE void set_shader_input(CPT_InternalName id, Texture* tex, 
                                const SamplerState& sampler, int priority=0);

    INLINE void set_shader_input(CPT_InternalName id, Texture* tex, bool read, 
                                bool write, int z=-1, int n=0, int priority=0);

    INLINE void set_shader_input(CPT_InternalName id, Texture* tex, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, ShaderBuffer* buf, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const NodePath& np, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_float& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_double& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_int& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase4& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase3& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase2& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LMatrix4& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LMatrix3& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase4& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase3& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase2& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LMatrix4& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LMatrix3& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase4i& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase3i& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const PTA_LVecBase2i& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase4i& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase3i& v, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, const LVecBase2i& v, int priority=0);

    INLINE void set_shader_input(CPT_InternalName id, int n1, int n2, int n3=0,
                                                    int n4=0, int priority=0);
    INLINE void set_shader_input(CPT_InternalName id, PN_stdfloat n1, PN_stdfloat n2,
                                PN_stdfloat n3=0, PN_stdfloat n4=0, int priority=0);

    INLINE void set_winding_order(CullFaceAttrib::Mode mode, int priority=0);

    INLINE void set_transparency(TransparencyAttrib::Mode mode, int priority=0);
    
    INLINE void set_instance_count(size_t instance_count);

    INLINE void set_attrib(const RenderAttrib* attrib, int priority=0);

    template<class AttribType>
    INLINE bool get_attrib(const AttribType*& attrib) const;

    INLINE const RenderAttrib* get_attrib(int slot) const;
    INLINE const RenderState* get_state() const;

    INLINE operator const ShaderAttrib*() const;
    INLINE operator const RenderState*() const;

    INLINE operator CPT(ShaderAttrib)() const;
    INLINE operator CPT(RenderState)() const;

    INLINE ShaderState& operator =(const RenderState* state);

private:
    NodePath _np;
};


#include "shaderState.I"

#endif // __SHADERSTATE_H__