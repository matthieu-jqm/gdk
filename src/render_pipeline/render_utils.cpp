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

#include <panda3d/cardMaker.h>
#include <panda3d/camera.h>
#include <panda3d/orthographicLens.h>
#include <panda3d/graphicsStateGuardian.h>
#include <panda3d/graphicsEngine.h>

#include "render_utils.h"

using namespace std;


void create_2d_scene(NodePath root, NodePath& cam_np) {

    Camera* cam = new Camera("pipeline-output");

    OrthographicLens* lens = new OrthographicLens;
    static const PN_stdfloat left = -1.0f;
    static const PN_stdfloat right = 1.0f;
    static const PN_stdfloat bottom = -1.0f;
    static const PN_stdfloat top = 1.0f;
    lens->set_film_size(right - left, top - bottom);
    lens->set_film_offset((right + left) * 0.5, (top + bottom) * 0.5);
    lens->set_near_far(-1000, 1000);
    cam->set_lens(lens);

    cam_np = root.attach_new_node(cam);
}


void create_fullscreen_quad(NodePath root, NodePath& quad_np, 
                            const string& shader_file) {

    quad_np = root.attach_new_node(make_fullscreen_quad());

    PT(Shader) shader = Shader::load(SHADER_LANG,
                                    GDK_SHADER_PATH VERT_SHADER("quad"),
                                    shader_file);

    quad_np.set_shader(shader);
}


const ShaderAttrib* get_shader_attrib(NodePath node) {

    const RenderAttrib* attr = node.get_attrib(ShaderAttrib::get_class_type());
    return DCAST(ShaderAttrib, attr);
}

const Geom* get_null_geom() {
    
    static PT(Geom) geom;
    
    if (UNLIKELY(geom == nullptr)) {

        const GeomVertexFormat* fmt = GeomVertexFormat::get_v3();
        GeomVertexData* vdata = new GeomVertexData(
            "null", fmt, GeomEnums::UH_static);
        
        geom = new Geom(vdata);
    }
    
    return geom;
}


void dispatch_compute( GraphicsStateGuardian* gsg, 
                        const ShaderAttrib* attrib,
                        size_t x, size_t y, size_t z)
{
    
    const Shader *shader = attrib->get_shader();
    nassertv(shader != nullptr);
    nassertv(gsg != nullptr);

#ifndef NDEBUG
    if (shader->get_error_flag())
        return;
#endif
    string shader_name = shader->get_filename(Shader::ST_compute).get_basename();

    CPT(RenderState) state = RenderState::make(attrib);

    gsg->set_state_and_transform(state, TransformState::make_identity());

    gsg->push_group_marker(string("Compute ") + shader_name);
    gsg->dispatch_compute(x, y, z);
    gsg->pop_group_marker();
}


GraphicsBuffer* make_buffer(const FrameBufferProperties& fb_prop,
                            const std::string& name, size_t size_x, 
                            size_t size_y, int sort, GraphicsOutput* host)
{

    GraphicsEngine* engine = GraphicsEngine::get_global_ptr();

    return  DCAST(GraphicsBuffer, 
        engine->make_output(host->get_pipe(), 
                            name, sort, fb_prop, 
                            WindowProperties::size(size_x, size_y), 
                            GraphicsPipe::BF_refuse_window | 
                            GraphicsPipe::BF_refuse_parasite |
                            GraphicsPipe::BF_fb_props_optional |
                            GraphicsPipe::BF_resizeable,
                            host->get_gsg(), host));
}


void draw_fullscreen_quad(  GraphicsStateGuardian* gsg,
                            const RenderState* state,
                            int num_instance)
{

    static CPT(Geom) geom = nullptr;

    if (UNLIKELY(geom == nullptr))
        geom = make_fullscreen_quad()->get_geom(0);
    
    gsg->push_group_marker("drawing fullscreen quad");
    draw_geom(gsg, geom, state, num_instance);
    gsg->pop_group_marker();
}


void draw_geom(GraphicsStateGuardian* gsg, const Geom* geom,
                const RenderState* state, int num_instance)
{
    
    Thread* current_thread = Thread::get_current_thread();

    gsg->set_state_and_transform(state, TransformState::make_identity());

    CPT(Geom) munged_geom = geom;
	CPT(GeomVertexData) munged_data = munged_geom->get_vertex_data();
	PT(GeomMunger) munger = gsg->get_geom_munger(state, current_thread);

    munger->munge_geom(munged_geom, munged_data, true, current_thread);

	munged_geom->draw(gsg, munged_data, num_instance, true, current_thread);
}



PT(GeomNode) make_fullscreen_quad() {

    static CardMaker cm("fullscreen quad");
    cm.set_frame_fullscreen_quad();
    
    return DCAST(GeomNode, cm.generate());
}


Texture* make_render_texture(const string& name) {

    Texture* texture = new Texture(name);

    texture->set_wrap_u(SamplerState::WM_clamp);
    texture->set_wrap_v(SamplerState::WM_clamp);
    texture->set_minfilter(SamplerState::FT_nearest);
    texture->set_magfilter(SamplerState::FT_nearest);
    texture->set_anisotropic_degree(0);
    texture->set_keep_ram_image(false);

    return texture;
}


namespace glsl {

    vec3::vec3(const LVector3f& copy): LVector3f(copy) {}
    
    vec3::vec3::vec3(const LColor& copy): LVector3(copy.get_xyz()) {}

    vec3i::vec3i(const LVector3i& copy): LVector3i(copy) {}
    
    vec3& vec3::operator =(const LVector3f& copy) {
        LVector3f::operator =(copy);
        return *this;
    }

    vec3i& vec3i::operator =(const LVector3i& copy) {
        LVector3i::operator =(copy);
        return *this;
    }
}