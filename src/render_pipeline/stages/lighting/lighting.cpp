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

#include <panda3d/cullFaceAttrib.h>
#include <panda3d/cullTraverser.h>
#include <panda3d/cullHandler.h>
#include <panda3d/cullBinAttrib.h>
#include <panda3d/cullBinUnsorted.h>
#include <panda3d/geomDrawCallbackData.h>
#include <panda3d/displayRegionDrawCallbackData.h>
#include <panda3d/glgsg.h>

#include "shadowBufferManager.h"
#include "shadowBuffer.h"
#include "lightPool.h"
#include "renderPipeline.h"
#include "render_utils.h"
#include "pointLightNode.h"
#include "spotLightNode.h"
#include "config_render_pipeline.h"
#include "lighting.h"

using namespace std;

DEFINE_TYPEHANDLE(Lighting)


ConfigVariableInt max_light_lods
("max-light-lods", 4,
    PRC_DESC("The maximum level of detail for light bouding volume."));


ConfigVariableInt max_lights_per_tile
("max-lights-per-tile", 128,
    PRC_DESC("The maximum number of lights that can be binned in one tile of"
            "the light volume"));



Lighting::Lighting():
    RenderScene("lighting"),
    _hw_conservative_raster(0),
    _fragment_shader_interlock(false),
    _half_tile_size(1, 0.0f),
    _screen_size(1, 0.0f),
    _view_scale(1, 0.0f),
    _num_lights(LT_COUNT, 0)
{

    _fb_props.set_rgb_color(false);
    _fb_props.set_depth_bits(0);
    _fb_props.set_rgba_bits(0,0,0,0);

    add_bin("lights");
}


void Lighting::add_light_type(LightType type) {

    nassertv(_pipeline != nullptr)


    LightPoolBase* handler = LightPoolBase::make(type);

    if (handler == nullptr)
        return;

    ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();
    ShadowBuffer* buffer = mgr->get_buffer(handler->get_node_type());

    if (buffer != nullptr) {
        Texture* shadow_map = buffer->get_shadow_map();
        
        string name = "u_" + handler->get_name() + "ShadowMaps";
        _accumulate.set_shader_input(name, shadow_map);
    }

    _light_pools[type] = handler;
    _pipeline->register_node_handler(handler);
    handler->set_stage(this);
}


const RenderState* Lighting::get_binning_state() const {
    return _binning;
}


void Lighting::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);    
}


void Lighting::dimensions_changed(LVector2i size) {

    _screen_size[0] = size;
    _num_tile = get_ceil_size(size, SST_16);

    _view_scale[0].set_x((float)size.get_x() / (_num_tile.get_x() << 4));
    _view_scale[0].set_y((float)size.get_y() / (_num_tile.get_y() << 4));

    _half_tile_size[0].set_x(1.0f / _num_tile.get_x());
    _half_tile_size[0].set_y(1.0f / _num_tile.get_y());


    RenderScene::dimensions_changed(_num_tile);
}


void Lighting::write_datagram(BamWriter* manager, Datagram& me) {
    
    RenderScene::write_datagram(manager, me);

    manager->write_pointer(me, _geom_normal);
    manager->write_pointer(me, _light_volume);
    manager->write_pointer(me, _linear_depth);
    manager->write_pointer(me, _accumulator);
    manager->write_pointer(me, _diffuse);
    manager->write_pointer(me, _normal);
    manager->write_pointer(me, _specular);
    manager->write_pointer(me, _depth);
}


void Lighting::fillin(DatagramIterator& scan, BamReader* manager) {
    
    RenderScene::fillin(scan, manager);
    manager->read_pointers(scan, 8);
}


int Lighting::complete_pointers(TypedWritable** p_list, BamReader* reader) {

    int n = RenderScene::complete_pointers(p_list, reader);

    _geom_normal = DCAST(Texture, p_list[n++]);
    _light_volume = DCAST(Texture, p_list[n++]);
    _linear_depth = DCAST(Texture, p_list[n++]);
    _accumulator = DCAST(Texture, p_list[n++]);
    _diffuse = DCAST(Texture, p_list[n++]);
    _normal = DCAST(Texture, p_list[n++]);
    _specular = DCAST(Texture, p_list[n++]);
    _depth = DCAST(Texture, p_list[n++]);

    return n;
}


void Lighting::setup_shaders() {
 
    GraphicsStateGuardian* gsg = _pipeline->get_gsg();

    // before loading shaders we need to check the GPU capabilities

    if (gsg->has_extension("GL_NV_conservative_raster"))
        _hw_conservative_raster = GL_CONSERVATIVE_RASTERIZATION_NV;
    else if (gsg->has_extension("GL_INTEL_conservative_rasterization"))
        _hw_conservative_raster = GL_CONSERVATIVE_RASTERIZATION_INTEL;

    if (gsg->has_extension("GL_ARB_fragment_shader_interlock"))
        _fragment_shader_interlock = true;
    else
        nassert_raise("fragment shader interlock is not supported by the GPU," 
                "light stage is disabled.");

    // if (_hw_conservative_raster)
    if (true)
        _binning.set_shader(Shader::load(SHADER_LANG, 
                GDK_SHADER_PATH VERT_SHADER("lightBinning"),
                GDK_SHADER_PATH FRAG_SHADER("lightBinning")));
    else {
        _binning.set_shader(Shader::load(SHADER_LANG, 
                GDK_SHADER_PATH VERT_SHADER("lightConservative"),
                GDK_SHADER_PATH FRAG_SHADER("lightBinningConservative"),
                GDK_SHADER_PATH GEOM_SHADER("conservative")));

        _binning.set_shader_input("u_HalfPixelSize", _half_tile_size);
        _binning.set_shader_input("u_viewScale", _view_scale);
    }


    _accumulate.set_shader(Shader::load_compute(SHADER_LANG, 
                        GDK_SHADER_PATH COMP_SHADER("accumulateLighting")));
    
    _clear.set_shader(Shader::load_compute(SHADER_LANG,
                        GDK_SHADER_PATH COMP_SHADER("clearLightVolume")));

    _culling.set_shader(Shader::load_compute(SHADER_LANG,
                        GDK_SHADER_PATH COMP_SHADER("cullLights")));
}


void Lighting::setup_inputs() {
    
    const SceneData* data = _pipeline->get_scene_data();

    _culling.set_shader_input("u_linearDepth", _linear_depth);
    _culling.set_shader_input("u_lightVolume", _light_volume);

    _accumulate.set_shader_input("u_nearFar", data->_near_far);
    _accumulate.set_shader_input("u_geomNormal", _geom_normal);
    _accumulate.set_shader_input("u_linearDepth", _linear_depth);
    _accumulate.set_shader_input("u_screenSize", _screen_size);
    _accumulate.set_shader_input("u_lightVolume", _light_volume);
    _accumulate.set_shader_input("u_accumulator", _accumulator);
    _accumulate.set_shader_input("u_diffuse", _diffuse);
    _accumulate.set_shader_input("u_specular", _specular);
    _accumulate.set_shader_input("u_normal", _normal);
    _accumulate.set_shader_input("u_depth", _depth);


    _binning.set_flag(ShaderAttrib::F_hardware_instancing, true);
    _binning.set_bin("lights", 0);
    _binning.set_shader_input("u_lightVolume", _light_volume);
    _binning.set_shader_input("u_linearDepth", _linear_depth);
    _binning.set_shader_input("u_nearFar", data->_near_far);
    _binning.set_depth_write(false);
    _binning.set_depth_test(false);
    _binning.set_two_sided(true);

    _clear.set_shader_input("u_lightVolume", _light_volume, false, true, -1, 0);
    _clear.set_shader_input("u_lightCount", _num_lights);
}


void Lighting::setup_textures() {
    
    if (_light_volume == nullptr)
        return;

    _light_volume->setup_2d_texture_array(  _num_tile.get_x(),
                                            _num_tile.get_y(), 
                                            max_lights_per_tile,
                                            Texture::T_unsigned_int,
                                            Texture::F_rgba32i);
}


void Lighting::collect_data() {
    
    for (int type=0; type<LT_COUNT; ++type) {

        LightPoolBase* handler = _light_pools[type];

        if (handler == nullptr)
            continue;
        
        vector_uchar data;
        string name = handler->get_name() + "LightBuffer";

        handler->collect_data(data, _num_lights[type]);

        _accumulate.set_shader_input(name,
            new ShaderBuffer(name, std::move(data),
                            GeomEnums::UH_dynamic));
    }
}


TypedWritable* Lighting::make_from_bam(const FactoryParams& params) {
        
    Lighting* me = new Lighting;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}



void Lighting::setup() {

    LVector2i size = _pipeline->get_size();
    _num_tile = get_ceil_size(size, SST_16);

    setup_shaders();
    setup_textures();
    setup_inputs();

    add_light_type(LT_point);
    add_light_type(LT_spot);
    add_light_type(LT_directional);

    setup_buffer(_num_tile);
    dimensions_changed(size);
}


void Lighting::release() {

    for (PT(LightPoolBase)& handler: _light_pools) {
        if (handler != nullptr) {
            _pipeline->unregister_node_handler(handler);
            handler = nullptr;
        }
    }
}


void Lighting::render(Thread* current_thread) {

    collect_data();


    GraphicsStateGuardian* gsg = _buffer->get_gsg();
    LVecBase2i groups = get_ceil_size(_num_tile, SST_16);

    dispatch_compute(gsg, _clear, groups.get_x(), groups.get_y());


    if (_hw_conservative_raster)
        glEnable(_hw_conservative_raster);

    RenderScene::render(current_thread);

    if (_hw_conservative_raster)
        glDisable(_hw_conservative_raster);


    dispatch_compute(gsg, _culling, groups.get_x(), groups.get_y());
    dispatch_compute(gsg, _accumulate, _num_tile.get_x(), _num_tile.get_y());
}
