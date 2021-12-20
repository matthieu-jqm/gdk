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

#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include "renderScene.h"
#include "shaderState.h"
#include "sceneData.h"
#include "lightData.h"
#include "utils.h"

class LightPoolBase;

extern ConfigVariableInt max_light_lods;
extern ConfigVariableInt max_lights_per_tile;


class Lighting: public RenderScene {

    REGISTER_TYPE("Lighting", RenderScene)

public:
    using TileSize = ShortSizePoT;

    Lighting();
    virtual ~Lighting() = default;

    void add_light_type(LightType type);

    static void register_with_read_factory();

    const RenderState* get_binning_state() const;

protected:
    virtual void setup() override;
    virtual void release() override;
    virtual void render(Thread* current_thread) override;

    virtual void dimensions_changed(LVector2i size) override;

    void write_datagram(BamWriter* manager, Datagram& me);
    void fillin(DatagramIterator& scan, BamReader* manager);
    int complete_pointers(TypedWritable** p_list, BamReader* reader);

    PT(Texture) _light_volume;
    PT(Texture) _diffuse;
    PT(Texture) _accumulator;
    PT(Texture) _linear_depth;
    PT(Texture) _specular;
    PT(Texture) _geom_normal;
    PT(Texture) _normal;
    PT(Texture) _depth;

private:
    void setup_shaders();
    void setup_inputs();
    void setup_textures();

    void collect_data();

    static TypedWritable* make_from_bam(const FactoryParams& params);

    int _hw_conservative_raster;
    bool _fragment_shader_interlock;

    ShaderState _culling;
    ShaderState _accumulate;
    ShaderState _binning;
    ShaderState _clear;

    PTA_LVecBase2f _view_scale;
    PTA_LVecBase2i _screen_size;
    PTA_LVecBase2f _half_tile_size;

    PTA_int _num_lights;

    LVector2i _num_tile;

    typedef std::array<PT(LightPoolBase), LT_COUNT> LightPools;
    LightPools _light_pools;
};


#endif // __LIGHTING_H__