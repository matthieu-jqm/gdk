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

#ifndef __SHADERMATERIAL_H__
#define __SHADERMATERIAL_H__

#include <panda3d/namable.h>
#include <panda3d/bamReaderParam.h>
#include <panda3d/texturePool.h>
#include <panda3d/shader.h>
#include <limits>
#include <string>
#include <map>

#include "config_material.h"
#include "serializable.h"
#include "utils.h"


template<class T>
using is_numeric = std::enable_if_t<std::is_arithmetic<T>::value>;

template<class T>
using is_integer = std::enable_if_t<std::is_integral<T>::value>;

template<class T, class V>
using is_type = std::enable_if_t<std::is_same<T, V>::value>;


class ShaderMaterial;


template<class T>
class _MaterialProperty: public Property<T> {

    REGISTER_TEMPLATE("_MaterialProperty", Property<T>)

public:
    _MaterialProperty(  const std::string& name, 
                        const T& defaultv=T());

    const InternalName* get_id() const;

protected:
    CPT_InternalName _id;
};



template<class T, typename Enabler = void>
class MaterialProperty: public _MaterialProperty<PointerToArray<T>> {

public:
    MaterialProperty(ShaderMaterial* material, const std::string& name, 
                    const T& defaultv=T());

    virtual ~MaterialProperty() = default;

    INLINE T get_value() const;
    INLINE void set_value(const T& value);

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;

    INLINE MaterialProperty<T>& operator =(const T& value);
};



template<class T>
class MaterialProperty<T, is_numeric<T>>: public _MaterialProperty<PointerToArray<T>> {

public:
    MaterialProperty(ShaderMaterial* material, const std::string& name,
                    const T& minv = std::numeric_limits<T>::min(),
                    const T& maxv = std::numeric_limits<T>::max(),
                    const T& defaultv=T());

    virtual ~MaterialProperty() = default;

    INLINE T get_min() const;
    INLINE T get_max() const;

    INLINE T get_value() const;
    INLINE void set_value(const T& value);

    INLINE MaterialProperty<T>& operator =(const T& value);

private:
    T _min, _max;
};



template<class T>
class MaterialProperty<T, is_type<T, ShaderBuffer>>: public _MaterialProperty<PT(T)> {

public:
    MaterialProperty(ShaderMaterial* material, const std::string& name, 
                    ShaderBuffer* defaultv=nullptr);

    virtual ~MaterialProperty() = default;

    INLINE void set_value(ShaderBuffer* buffer);

    INLINE MaterialProperty<T>& operator =(ShaderBuffer* buffer);

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me);

private:
    ShaderMaterial* _material;
};



template<class T>
class MaterialProperty<T, is_type<T, Texture>>: public _MaterialProperty<PT(T)> {

public:
    MaterialProperty(ShaderMaterial* material, const std::string& name, 
                    Texture* defaultv=nullptr);

    virtual ~MaterialProperty() = default;

    INLINE void set_value(Texture* texture);
    INLINE void set_value(Filename filename);

    INLINE MaterialProperty<T>& operator =(Texture* texture);

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me);

private:
    ShaderMaterial* _material;
    ShaderInput::ShaderInputType _mode;
};








#define SHADER_PROP(type, name, ...) \
    MaterialProperty<type>& MACRO_CONCAT(_prop_, __COUNTER__) \
        = make_property<type>(name __VA_ARGS__);


#define SHADER_FLAG(flag) \
    ::Null MACRO_CONCAT(_flag_, __COUNTER__) = make_flag(ShaderAttrib::flag);


#define SHADING_LANGUAGE(lang) \
    static constexpr Shader::ShaderLanguage _get_shading_language() { \
        return Shader::SL_ ## lang; \
    }

#define _SHADER_PATH(type, path) \
    static constexpr const char* _get_ ## type ## _shader() { \
        return path; \
    }

#define MAT_ID(id) CAT(id, _hash)
#define MAT(id) MaterialDef<MAT_ID(id)>

#define MATERIAL(id) \
    template<> \
    INLINE std::string get_template_name<MAT_ID(id)>() { \
        return id; \
    } \
    template<> \
    struct MAT(id) : public MaterialBase<MAT_ID(id)>



#define VERTEX_SHADER(path)             _SHADER_PATH(vertex, path)
#define FRAGMENT_SHADER(path)           _SHADER_PATH(fragment, path)
#define GEOMETRY_SHADER(path)           _SHADER_PATH(geometry, path)
#define TESS_CONTROL_SHADER(path)       _SHADER_PATH(tess_control, path)
#define TESS_EVALUATION_SHADER(path)    _SHADER_PATH(tess_evaluation, path)



class ShaderMaterial: public Namable, public Serializable {

    REGISTER_TYPE("ShaderMaterial", Namable, Serializable)

public:
    typedef Shader::ShaderLanguage ShaderLanguage;
    typedef Shader::ShaderFile ShaderFile;

    virtual ~ShaderMaterial() = default;
    void set_shader_input(const ShaderInput& input);

    operator const ShaderAttrib*() const;
    const ShaderAttrib* get_attrib() const;

    void set_flag(int flag, bool value);

protected:
    ShaderMaterial(const std::string& name="");

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;

    template<class T, class... Args>
    MaterialProperty<T>& make_property(const std::string& name, Args... args);

    ::Null make_flag(int flag);

    CPT(ShaderAttrib) _attrib;
};



template<uint32_t>
class MaterialDef {};

template<uint32_t T>
class MaterialBase: public ShaderMaterial {

    REGISTER_TEMPLATE("Material", ShaderMaterial)

public:
    virtual ~MaterialBase() = default;

    static void register_with_read_factory();

protected:
    MaterialBase();

    SHADING_LANGUAGE(none)

    VERTEX_SHADER("")
    FRAGMENT_SHADER("")
    GEOMETRY_SHADER("")
    TESS_CONTROL_SHADER("")
    TESS_EVALUATION_SHADER("")

    static TypedWritable* make_from_bam(const FactoryParams& params);
    static TypedWritable* make(const FactoryParams& params);
};


#include "shaderMaterial.T"


#endif // __SHADERMATERIAL_H__