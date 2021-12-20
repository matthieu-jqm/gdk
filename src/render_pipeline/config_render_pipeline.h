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

#ifndef __CONFIG_RENDER_PIPELINE_H__
#define __CONFIG_RENDER_PIPELINE_H__

#include <panda3d/configVariableInt.h>
#include <panda3d/notifyCategoryProxy.h>
#include "shaderMaterial.h"
#include "config_gdk.h"

#define USE_SHADER_LOADER_HACK 1

NotifyCategoryDeclNoExport(render_pipeline);

#ifdef NOTIFY_DEBUG
    // Non-release build:
    #define rp_debug(msg) \
        if (render_pipeline_cat.is_debug()) \
            render_pipeline_cat->debug() << msg << std::endl; \
        else {}
#else
    // Release build:
    #define render_pipeline_debug(msg) ((void)0);
#endif

#define rp_info(msg) \
  render_pipeline_cat->info() << msg << std::endl

#define rp_warning(msg) \
  render_pipeline_cat->warning() << msg << std::endl

#define rp_error(msg) \
  render_pipeline_cat->error() << msg << std::endl


MATERIAL("PBR standard opaque") {

    SHADING_LANGUAGE(GLSL)
    VERTEX_SHADER(GDK_SHADER_PATH "PBR.vert")
    FRAGMENT_SHADER(GDK_SHADER_PATH "PBR.frag")

    SHADER_PROP(Texture, "albedo")
    SHADER_PROP(Texture, "roughness")    
    SHADER_PROP(Texture, "metalness")
    SHADER_PROP(Texture, "normal map")  
};


void init_render_pipeline();


#endif // __CONFIG_RENDER_PIPELINE_H__