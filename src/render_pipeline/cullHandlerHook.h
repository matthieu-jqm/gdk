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

#ifndef __CULLHANDLERHOOK_H__
#define __CULLHANDLERHOOK_H__

#include <panda3d/cullHandler.h>
#include <panda3d/renderAttrib.h>
#include <panda3d/cullableObject.h>
#include <panda3d/cullTraverser.h>

#include "utils.h"


class RenderPipeline;
class PipelineCullTraverser;


class NodeHandler: public TypedReferenceCount {

    REGISTER_TYPE("NodeHandler", TypedReferenceCount)

public:
    NodeHandler() = default;
    virtual ~NodeHandler() = default;

    virtual void record_node(CullTraverserData& data,
                            const PipelineCullTraverser* trav) = 0;
    
    virtual void post_cull(const PipelineCullTraverser* trav);
    virtual void setup(RenderPipeline* pipeline);
    virtual void cleanup(RenderPipeline* pipeline);

    virtual TypeHandle get_node_type() const = 0;
};


class PipelineCullTraverser: public CullTraverser {

    REGISTER_TYPE("PipelineCullTraverser", CullTraverser)

public:
    PipelineCullTraverser(RenderPipeline* pipeline);
    virtual ~PipelineCullTraverser() = default;

    virtual void set_scene(SceneSetup* scene_setup,
                         GraphicsStateGuardianBase* gsg,
                         bool dr_incomplete_render) override;

    virtual void end_traverse() override;

    void record_node(CullTraverserData& data);

    RenderPipeline* get_pipeline() const;

private:
    RenderPipeline* _pipeline;
};


#include "cullHandlerHook.T"


#endif // __CULLHANDLERHOOK_H__