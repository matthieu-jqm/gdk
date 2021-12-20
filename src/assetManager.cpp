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

#include <panda3d/modelPool.h>

// #include "defines.h"
#include "utils.h"
#include "assetManager.h"

using namespace std;

Singleton<AssetManager> AssetManager::_global_ptr;
LightMutex AssetManager::_lock;


AssetManager* AssetManager::ptr() {
    
    if (UNLIKELY(!_global_ptr))
        _global_ptr = new AssetManager;

    return _global_ptr;
}



PT(PandaNode) AssetManager::make_asset(TypeHandle type, const string& name,
                                        MatchType match)
{
    return AssetManager::ptr()->ns_make_asset(type, name, match);
}

PandaNode* AssetManager::get_asset(TypeHandle type, const string& name,
                                        MatchType match)
{
    return AssetManager::ptr()->ns_get_asset(type, name, match);
}


string AssetManager::get_location(TypeHandle type, MatchType match) {
    LightMutexHolder holder(_lock);

    return AssetManager::ptr()->ns_get_location(type, match);
}


void AssetManager::set_location(TypeHandle type, const string& location) {
    LightMutexHolder holder(_lock);

    AssetManager::ptr()->ns_set_location(type, location);
}


string AssetManager::get_default_location() {
    LightMutexHolder holder(_lock);

    return AssetManager::ptr()->ns_get_default_location();
}


void AssetManager::set_default_location(const string& location) {
    LightMutexHolder holder(_lock);

    AssetManager::ptr()->ns_set_default_location(location);
}


string AssetManager::ns_get_location(TypeHandle type, MatchType match) const {    

    return find(_locations, type, match, _default_location);
}


void AssetManager::ns_set_location(TypeHandle type, const string& location) {

    _locations[type] = location;
}


string AssetManager::ns_get_default_location() const {

    return _default_location;
}

void AssetManager::ns_set_default_location(const string& location) {

    _default_location = location;
}


PT(PandaNode) AssetManager::ns_make_asset(TypeHandle type, const string& name,
                                            MatchType match)
{
    PandaNode* asset = ns_get_asset(type, name, match);

    if (asset != nullptr)
        return asset->copy_subgraph();
    
    return nullptr;
}

PandaNode* AssetManager::ns_get_asset(TypeHandle type, const string& name,
                                        MatchType match)
{
    Filename path(ns_get_location(type, match));

    path.set_basename_wo_extension(name);
    path.set_extension("bam");

    LoaderOptions options(LoaderOptions::LF_no_disk_cache);
    ModelRoot* model = ModelPool::load_model(path, options);
        
    if (model && model->is_exact_type(ModelRoot::get_class_type())) {
        nassertr(model->get_num_children() > 0, nullptr);

        return model->get_child(0);
    }

    return model;
}

