// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// This file is part of projet.
// 
// projet is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// projet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with projet.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __UTILS_H__
#define __UTILS_H__

#include <panda3d/genericAsyncTask.h>
#include <panda3d/eventHandler.h>
#include <panda3d/nodePath.h>
#include <functional>

#include "assetManager.h"
#include "easing.h"
#include "localRef.h"


#ifdef _WIN32
    #define EXPORT_IMP __declspec(dllexport)
    #define IMPORT_IMP __declspec(dllimport)
#else
    #define EXPORT_IMP __attribute__((visibility("default")))
    #define IMPORT_IMP
#endif

#ifdef PLUGIN
    #define DL_EXPORT IMPORT_IMP
#else
    #define DL_EXPORT EXPORT_IMP
#endif



#define CAT(A, B) A ## B
#define MACRO_CONCAT(a, b) CAT(a,b)
#define SELECT(NAME, NUM) CAT(NAME ## _, NUM)
#define COMPOSE(NAME, ARGS) NAME ARGS

#define GET_COUNT(_0, _1, _2, _3, _4, _5, _6, COUNT, ...) COUNT
#define EXPAND() ,,,,,,

#define VA_SIZE(...) COMPOSE(GET_COUNT, (EXPAND __VA_ARGS__ (),0,6,5,4,3,2,1))
#define FOR_EACH(M, ...) SELECT(ARGS, VA_SIZE(__VA_ARGS__))(M, __VA_ARGS__)
#define VA_SELECT(M, ...) SELECT(M, VA_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define ARGS_0(M, X)      
#define ARGS_1(M, X)      M(X)
#define ARGS_2(M, X, ...) M(X)ARGS_1(M, __VA_ARGS__)
#define ARGS_3(M, X, ...) M(X)ARGS_2(M, __VA_ARGS__)
#define ARGS_4(M, X, ...) M(X)ARGS_3(M, __VA_ARGS__)
#define ARGS_5(M, X, ...) M(X)ARGS_4(M, __VA_ARGS__)
#define ARGS_6(M, X, ...) M(X)ARGS_5(M, __VA_ARGS__)



#define INIT_TYPE(type) type::init_type();
#define TYPE_CLASS(type) ,type::get_class_type()



#define REGISTER_TYPE_WO_INIT \
public: \
    static TypeHandle get_class_type() { \
        return _type_handle; \
    } \
    virtual TypeHandle get_type() const { \
        return get_class_type(); \
    } \
    virtual TypeHandle force_init_type() { \
        init_type(); \
        return get_class_type(); \
    } \
private: \
    static TypeHandle _type_handle;



#define _REGISTER_TYPE(type, post, ...) \
public: \
    static void init_type() { \
        FOR_EACH(INIT_TYPE, __VA_ARGS__) \
        register_type(_type_handle, type \
            FOR_EACH(TYPE_CLASS, __VA_ARGS__)); \
        post; \
    } \
    REGISTER_TYPE_WO_INIT


#define REGISTER_ATTRIB(type, ...) \
    _REGISTER_TYPE(type, init_slot(), __VA_ARGS__) \
public: \
    virtual int get_slot() const override { \
        return get_class_slot(); \
    } \
    static int get_class_slot() { \
        return _attrib_slot; \
    } \
private: \
    static int _attrib_slot;


#define REGISTER_TYPE(type, ...) \
    _REGISTER_TYPE(type, post_init(), __VA_ARGS__)

#define REGISTER_TEMPLATE(type, ...) \
    _REGISTER_TYPE(template_name(type, get_template_name<T>()), \
                post_init(), __VA_ARGS__)


#define DEFINE_TYPEHANDLE(type) TypeHandle type::_type_handle;

#define DEFINE_ATTRIB(type) \
    DEFINE_TYPEHANDLE(type) \
    int type::_attrib_slot;


#define _REGISTER_TEMPLATE_NAME(type, name) \
    template<> \
    INLINE std::string get_template_name<type>() { \
        return name; \
    } \

#define REGISTER_TEMPLATE_NAME(type) _REGISTER_TEMPLATE_NAME(type, #type)



#define CHECK_MEMBER_FN(name) template<class T> \
    struct has_member_fn<T, decltype(std::declval<T>().name(), void())>: \
        std::true_type {};


#define HAS_NOT_MEMBER(name) template<class T, typename = void> \
    struct check_ ## name

#define HAS_MEMBER(name) template<class T> \
    struct check_ ## name <T, typename \
    std::enable_if<has_member_fn<T>::value>::type>



INLINE void post_init() {}




template<class Type>
INLINE std::string get_template_name() {
    return get_type_handle(Type).get_name();
}

template<size_t N>
INLINE std::string get_template_name() {
    return std::to_string(N);
}


size_t unregister_task(const std::string& name, void* data=nullptr);


bool register_hook(EventHandler::EventCallbackFunction* func,
                void* data, const std::string& name);


PandaNode* find_parent(PandaNode* node, TypeHandle type);

TypedWritable* read_object(Filename filename);

bool write_object(Filename filename, const TypedWritable* object);



enum ShortSizePoT {
    SST_1, SST_2, SST_4, SST_8, SST_16, SST_32, SST_64,
    SST_128, SST_256, SST_512, SST_1024, SST_2048, SST_4096,
    SST_8192, SST_16384, SST_32768, SST_65536, SST_invalid,
};


INLINE size_t as_numeric(ShortSizePoT size) {

    if (size == SST_invalid)
        return 0;

    return 0x1 << size;
}


INLINE size_t get_floor_size(size_t n, ShortSizePoT size) {
    
    return n >> size;
}


INLINE size_t get_ceil_size(size_t n, ShortSizePoT size) {

    return get_floor_size(n + as_numeric(size) - 1, size);
}


INLINE LVecBase2i get_floor_size(LVecBase2i n, ShortSizePoT size) {

    return LVecBase2i(  get_floor_size(n.get_x(), size),
                        get_floor_size(n.get_y(), size));
}


INLINE LVecBase2i get_ceil_size(LVecBase2i n, ShortSizePoT size) {

    return LVecBase2i(  get_ceil_size(n.get_x(), size),
                        get_ceil_size(n.get_y(), size));
}




template<class T>
INLINE T keep_lowest_bit_set(T value) {

    return static_cast<T>(value - (value & value - 1));
}


#if __cplusplus < 201703L

template<class T>
T clamp(T value, T min_value, T max_value) {

    return std::min(std::max(min_value, value), max_value);
}

#else

using std::clamp;

#endif


#if __cplusplus < 202003L

template<class InputIt, class T>
T accumulate(InputIt first, InputIt last, T init) {

    for (; first != last; ++first) {
        init = std::move(init) + *first; // std::move since C++20
    }
    return init;
}

#endif

INLINE float saturate(float value) {

    return clamp(value, 0.0f, 1.0f);
}


template<class T>
INLINE PT(T) make_asset(const std::string& name) {
    return DCAST(T, AssetManager::make_asset(T::get_class_type(), name));
}

template<class T>
INLINE T* get_asset(const std::string& name) {
    return DCAST(T, AssetManager::get_asset(T::get_class_type(), name));
}


INLINE float mapf(float min_a, float max_a, float min_b, float max_b, float x) {
    return lerp(min_b, max_b, (x - min_a) / (max_a - min_a));
}

INLINE double fract(double x) {

    return x - std::floor(x);
}


INLINE double hash(double seed) {

    double p = fract(p * 0.1031f);
    p *= p + 33.33f;
    p *= p + p;
    return fract(p);
}


INLINE int rand(int n) {
    return rand() % n;
}

INLINE float randf() {
    return (float)rand() / (float)RAND_MAX;
}

INLINE float randf(float min, float max) {
    return lerp(min, max, randf());
}


INLINE uint32_t hash_32(uint32_t a) {

    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}


INLINE float smoothstep(float a, float b, float  x) {

    float t = x * x * (3.0f - 2.0f * x); 
    return lerp(a, b, t);
}


INLINE float smootherstep(float a, float b, float x) {
    
    float t = x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
    return lerp(a, b, t);
}


INLINE float noise(double t) {

    int step = floor(t);

    float a = ::hash(step);
    float b = ::hash(step + 1);

    return smoothstep(a, b, t - step);
}


INLINE float noise_sin(double t, float factor) {

    int step = floor(t);

    float a = ::hash(step);
    float b = ::hash(step + 1);

    a = (a - 0.5f) * factor;
    b = (b - 0.5f) * factor;

    float base = step & 0x1 ? 1.0f : -1.0f;

    return smoothstep(a + base, b - base, t - step);
}


constexpr uint32_t fnv1a_32(char const* val, size_t n) {
    return ((n ? fnv1a_32(val, n - 1) : 2166136261u) ^ val[n]) * 16777619u;
}

constexpr uint32_t operator"" _hash(char const* val, size_t n) {
    return fnv1a_32(val, n);
}


INLINE std::string template_name(const std::string& cls, const std::string& tmpl) {
    return cls + "<" + tmpl + ">";
}


template <class T, typename = void>
struct has_member_fn: std::false_type {};


template<class T>
class LocalNodePath: public T, public LocalRef<NodePath> {

    REGISTER_TEMPLATE("LocalNodePath", T, NodePath);

public:

    // template <class... Args,
    //     std::enable_if_t<std::is_constructible<T, Args&&...>::value, int> = 0>
    // LocalNodePath(Args&&... args):
    //     T(std::forward<Args>(args)...), LocalRef<NodePath>(this, this) {}

    template <class... Args>
    LocalNodePath(Args&&... args);
    virtual ~LocalNodePath() = default;

    // disambiguate NodePath and PandaNode

    using NodePath::get_name;
    using NodePath::set_name;
    using NodePath::set_effect;
    using NodePath::set_attrib;
    using NodePath::prepare_scene;
    using NodePath::get_transform;

    using PandaNode::write_datagram;
    using PandaNode::fillin;
};



template<class T>
class Singleton: public std::unique_ptr<T> {
public:
    Singleton() = default;
    Singleton(const Singleton<T>& copy) = default;
    Singleton(T* ptr);

    ~Singleton() = default;
    
    Singleton<T>& operator =(Singleton<T>&& copy) = default;

    INLINE Singleton<T>& operator =(T* ptr);
    INLINE operator T*() const;
};



template<class T>
class InstanceIndex {

public:
    InstanceIndex();
    virtual ~InstanceIndex();

    operator int() const;

private:
    static LightMutex _lock;
    static BitArray _indices;
    int _index;
};



template<class T>
class HandlerCache {

public:
    HandlerCache() = default;
    ~HandlerCache() = default;

    PT(T) lock(size_t index) const;
    bool has_handler(size_t index) const;
    void set_handler(size_t index, T* pointer);

private:
    std::vector<WPT(T)> _handlers;
};

class RefHolder {

public:
    RefHolder(ReferenceCount* object);
    ~RefHolder();

private:
    ReferenceCount* _object;
};

class Null {};

#include "utils.T"

#endif // __UTILS_H__