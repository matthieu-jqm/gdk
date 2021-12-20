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
#include <panda3d/bamFile.h>

#include "config_material.h"
#include "shaderMaterial.h"
#include "materialsPool.h"

using namespace std;


Singleton<MaterialsPool> MaterialsPool::_global_ptr;


MaterialsPool* MaterialsPool::get_ptr() {
    
    if (UNLIKELY(!_global_ptr))
        _global_ptr = new MaterialsPool;

    return _global_ptr;
}


ShaderMaterial* MaterialsPool::load_material(const Filename& filename) {
    return get_ptr()->ns_load_material(filename);
}


bool MaterialsPool::has_material(const Filename& filename) {
    return get_ptr()->ns_has_material(filename);
}


bool MaterialsPool::verify_material(const Filename& filename) {
    return load_material(filename) != nullptr;
}


ShaderMaterial* MaterialsPool::get_material(const Filename& filename) {
    return get_ptr()->ns_get_material(filename);
}


void MaterialsPool::add_material(const Filename& filename, ShaderMaterial* material) {
    get_ptr()->ns_add_material(filename, material);
}


void MaterialsPool::release_material(const Filename& filename) {
    get_ptr()->ns_release_material(filename);
}


void MaterialsPool::release_all_materials() {
    get_ptr()->ns_release_all_materials();
}


int MaterialsPool::garbage_collect() {
    return get_ptr()->ns_garbage_collect();
}


void MaterialsPool::list_contents(ostream& out) {
    get_ptr()->ns_list_contents(out);
}


void MaterialsPool::list_contents() {
    get_ptr()->ns_list_contents(cout);
}


void MaterialsPool::write(ostream& out) {
    get_ptr()->ns_list_contents(out);
}


bool MaterialsPool::ns_has_material(const Filename& filename) const {
    
    Materials::const_iterator it = _materials.find(filename);
    return it != _materials.end() && it->second != nullptr;
}


ShaderMaterial* MaterialsPool::ns_get_material(const Filename& filename)  {

    LightMutexHolder holder(_lock);
    Materials::const_iterator it = _materials.find(filename);

    if (it != _materials.end())
        return it->second;

    return nullptr;
}


ShaderMaterial* MaterialsPool::ns_load_material(const Filename& filename) {

    ShaderMaterial* material = ns_get_material(filename);
    
    if (material != nullptr) {
        mat_debug("ModelPool found cached material " 
                << material << " for " << filename);
        return material;
    } else
        mat_debug("ModelPool material " << filename << " not in cache");


    material = DCAST(ShaderMaterial, read_object(filename));

    if (material != nullptr)
        ns_add_material(filename, material);

    return material;
}


void MaterialsPool::ns_add_material(const Filename& filename, ShaderMaterial* material)  {
    
    LightMutexHolder holder(_lock);
    mat_debug("MaterialsPool storing " << material << " for " << filename);

    _materials[filename] = material;
}


void MaterialsPool::ns_release_material(const Filename& filename)  {

    LightMutexHolder holder(_lock);
    Materials::iterator it = _materials.find(filename);

    if (it != _materials.end())
        _materials.erase(it);   
}


void MaterialsPool::ns_release_all_materials()  {
    
    LightMutexHolder holder(_lock);
    _materials.clear();
}


int MaterialsPool::ns_garbage_collect()  {

    LightMutexHolder holder(_lock);
    int num_released = 0;

    Materials::const_iterator it = _materials.cbegin();

    while (it != _materials.cend()) {
        ShaderMaterial* material = it->second;

        if (material->get_ref_count() == 1) {

            mat_debug("Releasing " << it->first);
            ++num_released;
            _materials.erase(it++);

        } else
            ++it;
    }

    return num_released;
}


void MaterialsPool::ns_list_contents(ostream& out) const {

    LightMutexHolder holder(_lock);
    out << _materials.size() << " materials:" << endl;

    for (const Materials::value_type& it : _materials) {
        ShaderMaterial* material = it.second;
        out << "  " << it.first << " (count = " 
            << material->get_ref_count() << ")" << endl;
    }
}
