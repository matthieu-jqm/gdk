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

#include <panda3d/shaderAttrib.h>


INLINE ShaderState::ShaderState():
    _np("shader-state")
{
    
}

INLINE ShaderState::ShaderState(const ShaderState& copy):
    ShaderState(copy.get_state())
{

}

INLINE ShaderState::ShaderState(const RenderState* state):
    ShaderState()
{
    _np.set_state(state);
}

INLINE CPT(RenderState) ShaderState::make_copy() const {
    
    CPT(RenderState) new_state = RenderState::make_empty();
    return new_state->compose(_np.get_state());
}

INLINE void ShaderState::clear_shader() {
    _np.clear_shader();
}

INLINE bool ShaderState::has_shader() const {
    return _np.get_shader() != nullptr;
}

INLINE void ShaderState::set_bin(const std::string& name, int draw_order, int priority) {
    _np.set_bin(name, draw_order, priority);    
}

INLINE void ShaderState::set_depth_write(bool enable, int priority) {
    _np.set_depth_write(enable, priority);    
}

INLINE void ShaderState::set_depth_test(bool enable, int priority) {
    _np.set_depth_test(enable, priority);
}

INLINE void ShaderState::set_two_sided(bool enable, int priority) {
    _np.set_two_sided(enable, priority);
}

INLINE const RenderAttrib* ShaderState::get_attrib(int slot) const {
    return _np.node()->get_attrib(slot);    
}

template<class AttribType>
INLINE bool ShaderState::get_attrib(const AttribType*& attrib) const {
    return get_state()->get_attrib(attrib);
}

INLINE void ShaderState::set_attrib(const RenderAttrib* attrib, int priority) {
    _np.set_attrib(attrib, priority);
}

INLINE ShaderState& ShaderState::operator =(const RenderState* state) {
    _np.set_state(state);
    return *this;
}

INLINE ShaderState::operator const ShaderAttrib*() const {
    int slot = ShaderAttrib::get_class_slot();
    return DCAST(ShaderAttrib, get_attrib(slot));
}

INLINE ShaderState::operator const RenderState*() const {
    return _np.get_state();
}

INLINE ShaderState::operator CPT(ShaderAttrib)() const {
    int slot = ShaderAttrib::get_class_slot();
    return DCAST(ShaderAttrib, get_attrib(slot));
}

INLINE ShaderState::operator CPT(RenderState)() const {
    return _np.get_state();
}

INLINE const RenderState* ShaderState::get_state() const {
    return _np.get_state();
}

INLINE void ShaderState::set_shader(Shader* shader, int priority) {
    _np.set_shader(shader, priority);
}

INLINE const Shader* ShaderState::get_shader() const {
    return _np.get_shader();
}

INLINE void ShaderState::set_instance_count(size_t instance_count) {
    _np.set_instance_count(instance_count);
}

INLINE void ShaderState::set_shader_input(const ShaderInput& input) {
    _np.set_shader_input(input);
}

INLINE void ShaderState::set_shader_input(ShaderInput&& input) {
    _np.set_shader_input(input);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, Texture* tex, 
                                    const SamplerState& sampler, int priority)
{
    _np.set_shader_input(id, tex, sampler, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, Texture* tex, 
                            bool read, bool write, int z, int n, int priority)
{
    _np.set_shader_input(id, tex, read, write, z, n, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, Texture* tex, int priority) {
    _np.set_shader_input(id, tex, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, ShaderBuffer* buf, int priority) {
    _np.set_shader_input(id, buf, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const NodePath& np, int priority) {
    _np.set_shader_input(id, np, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_float& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_double& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_int& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase4& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase3& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase2& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LMatrix4& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LMatrix3& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase4& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase3& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase2& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LMatrix4& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LMatrix3& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase4i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase3i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const PTA_LVecBase2i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase4i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase3i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, const LVecBase2i& v, int priority) {
    _np.set_shader_input(id, v, priority);
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, int n1, int n2, 
                                                int n3, int n4, int priority)
{
    _np.set_shader_input(id, n1, n2, n3, n4, priority);    
}

INLINE void ShaderState::set_shader_input(CPT_InternalName id, PN_stdfloat n1, 
                PN_stdfloat n2, PN_stdfloat n3, PN_stdfloat n4, int priority)
{
    _np.set_shader_input(id, n1, n2, n3, n4, priority);
}

INLINE void ShaderState::set_winding_order(CullFaceAttrib::Mode mode, int priority) {
    set_attrib(CullFaceAttrib::make(mode), priority);
}

INLINE void ShaderState::set_transparency(TransparencyAttrib::Mode mode, int priority) {
    _np.set_transparency(mode, priority);
}

INLINE void ShaderState::set_flag(int flag, bool value) {
    
    const ShaderAttrib* sattr;
    if (get_attrib(sattr))
        set_attrib(sattr->set_flag(flag, value));
}

INLINE bool ShaderState::get_flag(int flag) const {
    
    const ShaderAttrib* sattr;
    if (get_attrib(sattr))
        return sattr->get_flag(flag);
    
    return false;
}

INLINE void ShaderState::set_color(LColor color, int priority) {
    _np.set_color(color, priority);
}

INLINE LColor ShaderState::get_color() const {
    return _np.get_color();
}

INLINE void ShaderState::compose(const RenderState* other) {

    CPT(RenderState) state = _np.get_state();
    _np.set_state(state->compose(other));    
}

INLINE void ShaderState::set_material_off(int priority) {
    _np.set_material_off(priority);
}

INLINE void ShaderState::set_light_off(int priority) {
    _np.set_light_off(priority);
}