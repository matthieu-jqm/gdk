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


template<uint32_t id>
DEFINE_TYPEHANDLE(MaterialBase<id>)

template<class T>
DEFINE_TYPEHANDLE(_MaterialProperty<T>)


template<class T>
_MaterialProperty<T>::_MaterialProperty(const std::string& name,
                                        const T& defaultv):
        Property<T>(name, defaultv),
        _id(name)
{

}


template<class T>
const InternalName* _MaterialProperty<T>::get_id() const {
    return _id;
}




template<class T, typename Enabler>
MaterialProperty<T, Enabler>::
MaterialProperty(ShaderMaterial* material, const std::string& name, const T& defaultv):
        _MaterialProperty<PointerToArray<T>>(name, defaultv)
{
    const InternalName* id = _MaterialProperty<PointerToArray<T>>::_id;

    ShaderInput input(id, Property<T>::_value);
    material->set_shader_input(input);
}


template<class T, typename Enabler>
INLINE T MaterialProperty<T, Enabler>::get_value() const {
    return Property<PointerToArray<T>>::_value[0];
}


template<class T, typename Enabler>
INLINE void MaterialProperty<T, Enabler>::set_value(const T& value) {
    Property<PointerToArray<T>>::_value[0] = value;
}


template<class T, typename Enabler>
INLINE MaterialProperty<T>& MaterialProperty<T, Enabler>::
operator =(const T& value) {
    set_value(value);
    return *this;
}


template<class T, typename Enabler>
void MaterialProperty<T, Enabler>::
fillin(DatagramIterator& scan, BamReader* manager) {
    generic_read_datagram(Property<PointerToArray<T>>::_value[0], scan);
}


template<class T, typename Enabler>
void MaterialProperty<T, Enabler>::
write_datagram(BamWriter* manager, Datagram& me) {
    generic_write_datagram(me, Property<PointerToArray<T>>::_value[0]);
}




template<class T>
MaterialProperty<T, is_numeric<T>>::
MaterialProperty(ShaderMaterial* material, const std::string& name, 
                 const T& minv, const T& maxv, const T& defaultv):
        _MaterialProperty<PointerToArray<T>>(name, defaultv),
        _min(minv),
        _max(maxv)
{

    const InternalName* id = _MaterialProperty<T>::_id;

    ShaderInput input(id, Property<PointerToArray<T>>::_value);
    material->set_shader_input(input);
}


template<class T>
INLINE T MaterialProperty<T, is_numeric<T>>::get_max() const {
    return _max;
}


template<class T>
INLINE T MaterialProperty<T, is_numeric<T>>::get_min() const {
    return _min;
}


template<class T>
INLINE T MaterialProperty<T, is_numeric<T>>::get_value() const {
    return Property<PointerToArray<T>>::_value[0];
}


template<class T>
INLINE void MaterialProperty<T, is_numeric<T>>::
set_value(const T& value) {
    Property<PointerToArray<T>>::_value[0] = clamp(_min, _max, value);
}


template<class T>
INLINE MaterialProperty<T>& MaterialProperty<T, is_numeric<T>>::
operator =(const T& value) {
    set_value(value);
    return *this;
}



template<class T>
MaterialProperty<T, is_type<T, ShaderBuffer>>::
MaterialProperty(ShaderMaterial* material, const std::string& name,
                ShaderBuffer* defaultv):
        _MaterialProperty<PT(T)>(name, defaultv),
        _material(material)
{
    
}


template<class T>
INLINE void MaterialProperty<T, is_type<T, ShaderBuffer>>::
set_value(ShaderBuffer* buffer) {

    const InternalName* id = _MaterialProperty<PT(T)>::_id;

    Property<PT(T)>::_value[0] = buffer;
    _material->set_shader_input(ShaderInput(id, buffer));
}


template<class T>
INLINE MaterialProperty<T>& MaterialProperty<T, is_type<T, ShaderBuffer>>::
operator =(ShaderBuffer* buffer) {
    set_value(buffer);
    return *this;
}


template<class T>
void MaterialProperty<T, is_type<T, ShaderBuffer>>::
write_datagram(BamWriter* manager, Datagram& me) {

    ShaderBuffer* buffer = Property<PT(T)>::_value;

    if (buffer != nullptr)
        (TypedWritable*)buffer->write_datagram(manager, me);
}


template<class T>
void MaterialProperty<T, is_type<T, ShaderBuffer>>::
fillin(DatagramIterator& scan, BamReader* manager) {

    FactoryParams params;
    params.add_param(new BamReaderParam(scan, manager));

    TypeHandle type = ShaderBuffer::get_class_type();
    WritableFactory* factory = BamReader::get_factory();

    TypedWritable* object = factory->make_instance(type, params);
    nassertv(object != nullptr)

    set_value(DCAST(ShaderBuffer, object));
}



template<class T>
MaterialProperty<T, is_type<T, Texture>>::
MaterialProperty(ShaderMaterial* material, const std::string& name,
                Texture* defaultv):
        _MaterialProperty<PT(T)>(name, defaultv),
        _material(material),
        _mode(ShaderInput::M_invalid)
{
    
}


template<class T>
INLINE void MaterialProperty<T, is_type<T, Texture>>::
set_value(Texture* texture) {

    const InternalName* id = _MaterialProperty<PT(T)>::_id;

    Property<PT(T)>::_value = texture;
    _material->set_shader_input(ShaderInput(id, texture));
}


template<class T>
INLINE void MaterialProperty<T, is_type<T, Texture>>::
set_value(Filename filename) {

    Texture* texture = TexturePool::load_texture(filename);
    if (texture != nullptr)
        set_value(texture);
}


template<class T>
INLINE MaterialProperty<T>& MaterialProperty<T, is_type<T, Texture>>::
operator =(Texture* texture) {

    set_value(texture);
    return *this;
}


template<class T>
void MaterialProperty<T, is_type<T, Texture>>::
write_datagram(BamWriter* manager, Datagram& me) {

    Texture* texture = Property<PT(T)>::_value;

    if (texture != nullptr)
        me.add_string(texture->get_fullpath());
}


template<class T>
void MaterialProperty<T, is_type<T, Texture>>::
fillin(DatagramIterator& scan, BamReader* manager) {

    set_value(scan.get_string());


    // _mode = scan.get_uint8();

    
    // Property<PT(T)>::_value[0] = texture;

    // if (PT(ShaderMaterial) material = _material.lock()) {

    //     const InternalName* id = PropertyBase::get_id();

    //     switch (_mode) {

    //     case ShaderInput::M_texture:
    //         material->set_shader_input(ShaderInput(id, texture));
    //         break;

    //     case ShaderInput::M_texture_image:
    //         material->set_shader_input(ShaderInput(id, texture, 
    //                         scan.get_bool(), scan.get_bool(), 
    //                         scan.get_int32(), scan.get_int32()));
    //         break;

    //     case ShaderInput::M_texture_sampler:
    //         SamplerState sampler;
    //         sampler.read_datagram(scan, manager);
    //         material->set_shader_input(ShaderInput(id, texture, sampler));
    //         break;

    //     default:
    //         nout << "error : unknwon texture input mode" << std::endl;
    //     }
    // }
}



template<class T, class... Args>
MaterialProperty<T>& ShaderMaterial::
make_property(const std::string& name, Args... args) {

    auto* property = new MaterialProperty<T>(this, name, args...);
    _properties[name] = property;

    return *property;
}



template<uint32_t id>
MaterialBase<id>::MaterialBase():
    ShaderMaterial(get_class_type().get_name())
{

    PT(Shader) shader = Shader::load(MaterialDef<id>::_get_shading_language(), 
                                    MaterialDef<id>::_get_vertex_shader(), 
                                    MaterialDef<id>::_get_fragment_shader(),
                                    MaterialDef<id>::_get_geometry_shader(),
                                    MaterialDef<id>::_get_tess_control_shader(), 
                                    MaterialDef<id>::_get_tess_evaluation_shader());
    
    _attrib = DCAST(ShaderAttrib, _attrib->set_shader(shader, 100));
}


template<uint32_t id>
void MaterialBase<id>::register_with_read_factory() {

    BamReader::get_factory()->register_factory(_type_handle, make_from_bam);    
}


template<uint32_t id>
TypedWritable* MaterialBase<id>::make_from_bam(const FactoryParams& params) {

    MaterialDef<id>* me = new MaterialDef<id>;
    DatagramIterator scan;
    BamReader* manager;

    if (params.get_num_params() > 0) {
        parse_params(params, scan, manager);
        me->fillin(scan, manager);
    }
    return me;
}
