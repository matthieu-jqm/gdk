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

#include <panda3d/dconfig.h>

#include "config_render_pipeline.h"
#include "renderPipeline.h"
#include "renderScene.h"
#include "downscaleDepth.h"
#include "lightPool.h"
#include "directionalLightPool.h"
#include "lighting.h"
#include "computeStage.h"


ConfigureDef(config_render_pipeline);

ConfigureFn(config_render_pipeline) {
    init_render_pipeline();
}

NotifyCategoryDef(render_pipeline, "");


void init_render_pipeline() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;

    RenderTarget::init_type();
    RenderPipeline::init_type();
    RenderScene::init_type();
    Lighting::init_type();
    PipelineCullTraverser::init_type();
    DownscaleDepth::init_type();
    ComputeStage::init_type();

    LightPool<LT_point>::init_type();
    LightPool<LT_spot>::init_type();
    DirectionalLightPool::init_type();

    RenderTarget::register_with_read_factory();
    RenderPipeline::register_with_read_factory();
    RenderScene::register_with_read_factory();
    Lighting::register_with_read_factory();
    DownscaleDepth::register_with_read_factory();
    ComputeStage::register_with_read_factory();

    MaterialDef<"PBR standard opaque"_hash>::init_type();
    MaterialDef<"PBR standard opaque"_hash>::register_with_read_factory();
}