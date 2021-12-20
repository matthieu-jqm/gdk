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

#ifndef __RENDERPIPELINE_H__
#define __RENDERPIPELINE_H__



#include "pipelineNode.h"
#include "utils.h"
#include "IOPort.h"
#include "graphicsCallback.h"
#include "cullHandlerHook.h"
#include "sceneData.h"
#include "typeHandler.h"


class RenderStage;
class NodeHandler;
class CullableObject;
class NodeInfoAttrib;
class RenderPipelineBase;


class RenderTarget: public Texture {

    REGISTER_TYPE("RenderTarget", Texture)

public:
    RenderTarget(const std::string& name="");
    virtual ~RenderTarget() = default;

    void set_scale(float scale);
    float get_scale() const;

    void update_size(LVector2i size);

    static void register_with_read_factory();

protected:
    virtual void do_write_datagram_body(CData* cdata,
                BamWriter* manager, Datagram& me) override;

    virtual void do_fillin_body(CData* cdata, DatagramIterator& scan,
                BamReader* manager) override;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);

    float _scale;
};


class RenderPipeline: public TypedWritableReferenceCount, private GraphicsCallback {

    REGISTER_TYPE("RenderPipeline", TypedWritableReferenceCount)

public:
    RenderPipeline(GraphicsOutput* host, 
                    LVecBase2i size=LVecBase2i(0.0f),
                    const std::string& name="render-pipeline");

    virtual ~RenderPipeline();

    void setup(GraphicsOutput* host, LVecBase2i size);

    const RenderState* get_initial_state() const;
    void set_initilal_state(const RenderState* state);

    GraphicsOutput* get_buffer() const;
    GraphicsOutput* get_host() const;
    GraphicsStateGuardian* get_gsg() const;

    size_t get_num_stage() const;
    RenderStage* get_stage(size_t n) const;

    void remove_stage(size_t n);
    void add_stage(RenderStage* stage);

    void hook_display_regions();

    LVecBase2i get_size() const;
    void set_size(LVecBase2i size);

    Texture* get_output() const;

    void set_tag_name(const std::string& index);
    std::string get_tag_name() const;

    SceneData* get_scene_data() const;

    void register_node_handler(NodeHandler* handler, bool subtypes=true);
    void unregister_node_handler(NodeHandler* handler, bool subtypes=true);
    NodeHandler* get_handler(TypeHandle type) const;

    const RenderState* get_state(const std::string& name) const;
    void set_state(const std::string& name, const RenderState* state);

    RenderTarget* get_render_target(const std::string& name);

    void draw_bins(GraphicsStateGuardian* gsg, Thread* current_thread);

    void garbage_collect();

    static void register_with_read_factory();
    PStatCollector& get_pcollector();

protected:
    RenderPipeline();

    virtual void render(RenderCallbackData* data) override;

    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual int complete_pointers(TypedWritable** p_list, BamReader* manager) override;

    typedef std::map<std::string, PT(RenderTarget)> RenderTargets;
    typedef std::list<PT(RenderStage)> Stages;

    RenderTargets _targets;
    Stages _stages;

    CPT(RenderState) _initial_state;
    PT(RenderTarget) _output;

    PT(PipelineCullTraverser) _traverser;
    PT(CallbackGraphicsWindow) _callback_buffer;
    PT(GraphicsOutput) _host;

    std::string _name;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);

    typedef TypeHandler<PT(NodeHandler)> NodeHandlers;

    PStatCollector _pcollector;
    NodeHandlers _node_handlers;
    std::string _tag_name;
    uint16_t _num_targets;

    PT(SceneData) _scene_data;

    friend class PipelineCullTraverser;
};


#endif // __RENDERPIPELINE_H__