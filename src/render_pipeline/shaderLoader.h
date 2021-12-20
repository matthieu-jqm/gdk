
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

#ifndef __SHADERLOADER_H__
#define __SHADERLOADER_H__

#include <panda3d/shader.h>

#include "config_render_pipeline.h"
#include "utils.h"


class ShaderLoader: public Shader {

public:

    static Shader* load(const Filename& vert,
                        const Filename& frag,
                        const Filename& geom="",
                        const Filename& tess_control="",
                        const Filename& tess_eval="");

    static Shader* load_compute(const Filename& filename);

    static void start();
    static void stop();

    static void reload_shader(Shader* shader);
    static void update(Filename filename);
    static void garbage_collect();

    static void add_directory(const Filename& filename);

    static void set_language(ShaderLanguage lang);

#if USE_SHADER_LOADER_HACK
    bool is_dependent(const Filename& filename) const;
#endif

private:
    static AsyncTask::DoneStatus poll(GenericAsyncTask*, void*);
    typedef std::set<Filename> Directories;

    static Directories _directories;
    static time_t _last_check;
    static LightMutex _lock;

    static ShaderLanguage _lang;
};

#endif // __SHADERLOADER_H__