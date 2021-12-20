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

#include <panda3d/graphicsEngine.h>
#include <panda3d/dataNodeTransmit.h>

#include "renderPipeline.h"
#include "render_utils.h"
#include "config_render_pipeline.h"
#include "utils.h"

#include "downscaleDepth.h"

using namespace std;
using RenderCallbackData = CallbackGraphicsWindow::RenderCallbackData;


DEFINE_TYPEHANDLE(DownscaleDepth)


ConfigVariableInt num_extra_depth_lods
("num-extra-depth-lods", 0,
    PRC_DESC("How many levels of detail to add to the downsampled"
    "depth buffer. The base depth lod is a 16x16 tile (lod 4)"));




DownscaleDepth::DownscaleDepth():
    RenderStage("downscale-depth"),
    _texel_size(1, 0.0f)
{

}

void DownscaleDepth::register_with_read_factory() {
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


void DownscaleDepth::update() {

    GraphicsStateGuardian* gsg = _pipeline->get_gsg();

    dispatch_compute(gsg, _linearize, _groups.get_x(), _groups.get_y());
    dispatch_compute(gsg, _downscale, _groups.get_x(), _groups.get_y());
}


void DownscaleDepth::dimensions_changed(LVector2i size) {

    _linear_depth->set_x_size(size.get_x());
    _linear_depth->set_y_size(size.get_y());

    _groups = get_ceil_size(size, SST_16);

    _texel_size[0].set_x(1.0f / size.get_x());
    _texel_size[0].set_y(1.0f / size.get_y());
}


void DownscaleDepth::setup() {
    
    const SceneData* data = _pipeline->get_scene_data();


    _linearize.set_shader(Shader::load_compute(SHADER_LANG, 
                GDK_SHADER_PATH COMP_SHADER("linearizeDepth")));

    _downscale.set_shader(Shader::load_compute(SHADER_LANG, 
                GDK_SHADER_PATH COMP_SHADER("downscaleDepth")));


    LVecBase2i size = _pipeline->get_size();

    _linear_depth->setup_2d_texture(size.get_x(), size.get_y(), 
                                Texture::T_float,
                                Texture::F_rg32);

    SamplerState sampler;

    sampler.set_max_lod(SST_16 + num_extra_depth_lods);
    sampler.set_magfilter(SamplerState::FT_nearest_mipmap_nearest);
    sampler.set_minfilter(SamplerState::FT_nearest_mipmap_nearest);
    sampler.set_wrap_u(SamplerState::WM_border_color);
    sampler.set_wrap_v(SamplerState::WM_border_color);

    _linear_depth->set_default_sampler(sampler);


    _linearize.set_shader_input("u_nearFar", data->_near_far);
    _linearize.set_shader_input("u_outputDepth", _linear_depth);
    _linearize.set_shader_input("u_inputDepth", _depth);
        
    _downscale.set_shader_input("u_nearFar", data->_near_far);
    _downscale.set_shader_input("u_texelSize", _texel_size);
    _downscale.set_shader_input("u_inputDepth", _linear_depth);
    _downscale.set_shader_input("u_outputDepth", _linear_depth, false, true, -1, 4);

    dimensions_changed(size);
}


void DownscaleDepth::write_datagram(BamWriter* manager, Datagram& me) {
    
    RenderStage::write_datagram(manager, me);
    manager->write_pointer(me, _linear_depth);
    manager->write_pointer(me, _depth);
}


void DownscaleDepth::fillin(DatagramIterator& scan, BamReader* manager) {
    
    RenderStage::fillin(scan, manager);
    manager->read_pointers(scan, 2);
}


int DownscaleDepth::complete_pointers(TypedWritable** p_list, BamReader* reader) {
    
    int n = RenderStage::complete_pointers(p_list, reader);
    _linear_depth = DCAST(Texture, p_list[n++]);
    _depth = DCAST(Texture, p_list[n++]);

    return n;
}


TypedWritable* DownscaleDepth::make_from_bam(const FactoryParams& params) {

    DownscaleDepth* me = new DownscaleDepth;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}


    // for (int i=0; i<num_extra_depth_lods; ++i) {

    //     size_x = get_ceil_size(size_x, SST_2);
    //     size_y = get_ceil_size(size_y, SST_2);

    //     _downscale.set_shader_input("u_inputDepth", output, true, false, -1, i);
    //     _downscale.set_shader_input("u_outputDepth", output, false, true, -1, i+1);

    //     dispatch_compute(gsg, _downscale, size_x, size_y);
    // }