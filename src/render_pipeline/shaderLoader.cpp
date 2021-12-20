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

#include <panda3d/virtualFileSystem.h>
#include <panda3d/graphicsStateGuardian.h>
#include <panda3d/bamCache.h>

#include "config_gdk.h"
#include "render_utils.h"
#include "shaderLoader.h"

using namespace std;


LightMutex ShaderLoader::_lock;
time_t ShaderLoader::_last_check = time(nullptr);
ShaderLoader::Directories ShaderLoader::_directories;
Shader::ShaderLanguage ShaderLoader::_lang = SHADER_LANG;


bool is_same_path(const Filename& filename, Filename fn) {
    
    fn.standardize();
    fn.make_absolute();

    return filename == fn;
}


#if USE_SHADER_LOADER_HACK

bool is_dependent(const Filename& filename, Shader* shader) {

    // hack the pointer
    ShaderLoader* ptr = (ShaderLoader*)shader;
    return ptr->is_dependent(filename);
}


bool ShaderLoader::is_dependent(const Filename& filename) const {
    
    for (const Filename& source: _source_files) {
        if (is_same_path(filename, source))
            return true;
    }
    return false;
}

#else

bool is_dependent(const Filename& filename, Shader* shader) {


    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();
    DSearchPath path(get_model_path());

    // list source files
    for (int i = Shader::ST_none + 1; i < Shader::ST_COUNT; ++i) {
        Filename fn = shader->get_filename((Shader::ShaderType)i);

        if (!fn.empty()) {
            if (is_same_path(filename, fn))
                return true;

            path.prepend_directory(fn.get_dirname());
        }
    }

    // list included files
    for (int index=2048; ; ++index) {
        Filename fn = shader->get_filename_from_index(index, Shader::ST_none);

        if (fn == format_string(index))
            break; // a number means the end of the included files

        PT(VirtualFile) vf = vfs->find_file(fn, path);
        if (vf != nullptr && is_same_path(filename, vf->get_filename()))
            return true;
    }

    return false;
}

#endif




Filename format(Filename filename, Shader::ShaderType type) {
    
    if (filename.empty() || !filename.get_extension().empty())
        return filename;

    string ext;

    switch(type) {
        case Shader::ST_vertex: ext = VERT_SHADER_EXT; break;
        case Shader::ST_fragment: ext = FRAG_SHADER_EXT; break;
        case Shader::ST_geometry: ext = GEOM_SHADER_EXT; break;
        case Shader::ST_tess_control: ext = TESS_SHADER_EXT; break;
        case Shader::ST_tess_evaluation: ext = TESS_SHADER_EXT; break;
        case Shader::ST_compute: ext = COMP_SHADER_EXT; break;
        default:
            nassert_raise("unknown shader type");
    }
    filename.set_extension(ext);

    return filename;
}



void ShaderLoader::reload_shader(Shader* shader) {

    // first we have to clear the old ShaderContext so that the gsg can
    // recompile the new source file
    shader->release_all();
    shader->set_compiled(0, nullptr, 0);
    shader->_error_flag = false;

    Shader::ShaderFile sfile;
    sfile._vertex = shader->get_filename(Shader::ST_vertex);
    sfile._fragment = shader->get_filename(Shader::ST_fragment);
    sfile._geometry = shader->get_filename(Shader::ST_geometry);
    sfile._tess_control = shader->get_filename(Shader::ST_tess_control);
    sfile._tess_evaluation = shader->get_filename(Shader::ST_tess_evaluation);
    sfile._compute = shader->get_filename(Shader::ST_compute);
    sfile._shared = shader->get_filename(Shader::ST_COUNT);
    sfile._separate = shader->_text._separate;

    Shader* tmp_shader;
    shader->ref();

    // we need to remove if from the load table else the temp shader
    // we return the cached shader instead of reloading it
    ShaderTable::iterator lt = _load_table.find(sfile);
    ShaderTable::iterator mt = _make_table.find(shader->_text);

    if (lt != _load_table.end())
        _load_table.erase(lt);
    
    if (mt != _make_table.end())
        _make_table.erase(mt);

    ShaderLanguage lang = shader->get_language();

    if (!sfile._shared.empty())
        tmp_shader = Shader::load(sfile._shared, lang);
    else if (!sfile._compute.empty())
        tmp_shader = Shader::load_compute(lang, sfile._compute);
    else 
        tmp_shader = Shader::load(lang, sfile._vertex,
            sfile._fragment, sfile._geometry,
            sfile._tess_control, sfile._tess_evaluation);

    nassertv(tmp_shader != shader)

    if (tmp_shader == nullptr) {
        _load_table[sfile] = shader;

        if (cache_generated_shaders)
            _make_table[shader->_text] = shader;

        shader->unref();
    }

    // new the source has been reloaded, we can finally we can replace 
    // the old source and delete the temp shader

    shader->_text = tmp_shader->_text;
    _load_table[sfile] = shader;

    if (cache_generated_shaders)
        _make_table[shader->_text] = shader;

    shader->unref();
}


void ShaderLoader::garbage_collect() {

    LightMutexHolder holder(_lock);

    ShaderTable::const_iterator it = _load_table.cbegin();

    while (it != _load_table.cend()) {
        Shader* shader = it->second;

        if (shader->get_ref_count() == 1)
            _load_table.erase(it++);
        else
            ++it;
    }
}


void ShaderLoader::update(Filename filename) {

    // we need to make a copy of the table since new shaders will
    // be inserted in the table and we don't want the table to be modified
    // while we are iterating over it.
    ShaderTable table = _load_table;
    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();

    filename.standardize();
    filename.make_absolute();

    for (ShaderTable::value_type it : table) {
        Shader* shader = it.second;

        if (::is_dependent(filename, shader))
            reload_shader(shader);
    }
}


void ShaderLoader::add_directory(const Filename& filename) {
    LightMutexHolder holder(_lock);
    
    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();

    if (vfs->is_directory(filename))
        _directories.insert(filename);
}


void ShaderLoader::set_language(ShaderLanguage lang) {
    _lang = lang;    
}


AsyncTask::DoneStatus ShaderLoader::poll(GenericAsyncTask*, void*) {

    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();

    time_t next_time = _last_check;

    for (const Filename& filename: _directories) {

        PT(VirtualFileList) vfiles = vfs->scan_directory(filename);

        if (vfiles == nullptr)
            continue;
        
        for (int i=0; i<vfiles->size(); ++i) {
            VirtualFile* vfile = vfiles->get_file(i);

            if (!vfile->is_regular_file())
                continue;

            time_t timestamp = vfile->get_timestamp();

            if (timestamp >= _last_check) {
                next_time = max(next_time, timestamp + 1);
                update(vfile->get_filename());
            }
        }
    }
    _last_check = max(next_time, time(nullptr));

    return AsyncTask::DS_again;
}


void ShaderLoader::start() {

    stop();

    static ConfigVariableInt shader_watch_poll_rate
        ("shade-watch-poll-rate", 10,
        PRC_DESC("How many time per second to poll for modifications made"
                "to the shader files. higher value can slow down, but higher"
                "value will result in less reactive shader reloading"));

    static ConfigVariableList watch_shader_path
        ("watch-shader-path",
        PRC_DESC("Where to watch for shader modification, all files within"
                "that directory will be periodically polled for modification."
                "Whenever a shader file is modified, all shaders that loaded"
                "that file will be automatically reloaded"));

    
    size_t num_dirs = watch_shader_path.get_num_unique_values();

    for (size_t i=0; i<num_dirs; ++i)
        add_directory(watch_shader_path.get_unique_value(i));


    GenericAsyncTask* task = new GenericAsyncTask("poll-shaders", poll, nullptr);
    task->set_delay(1.0f / shader_watch_poll_rate);

    AsyncTaskManager* mgr = AsyncTaskManager::get_global_ptr();
    mgr->add(task);
}


void ShaderLoader::stop() {
    
    AsyncTaskManager* mgr = AsyncTaskManager::get_global_ptr();
    AsyncTask* task = mgr->find_task("poll-shaders");

    if (task != nullptr)
        task->remove();

    _directories.clear();
}


Shader* ShaderLoader::load(const Filename& vert, const Filename& frag,
                        const Filename& geom, const Filename& tess_control,
                        const Filename& tess_eval)
{

    PT(Shader) shader =  Shader::load(_lang,
                            format(vert, ST_vertex),
                            format(frag, ST_fragment),
                            format(geom, ST_geometry),
                            format(tess_control, ST_tess_control),
                            format(tess_eval, ST_tess_evaluation));
    
    if (shader != nullptr) {
        BamCache* cache = BamCache::get_global_ptr();
        shader->set_cache_compiled_shader(cache->get_cache_compiled_shaders());
    }

    return shader;
}


Shader* ShaderLoader::load_compute(const Filename& filename) {
    
    PT(Shader) shader = Shader::load_compute(_lang, filename);
    
    if (shader != nullptr) {
        BamCache* cache = BamCache::get_global_ptr();
        shader->set_cache_compiled_shader(cache->get_cache_compiled_shaders());
    }

    return shader;
}
