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


#include <panda3d/cullableObject.h>
#include <panda3d/shaderBuffer.h>
#include <panda3d/shaderAttrib.h>
#include <panda3d/omniBoundingVolume.h>
#include <panda3d/pStatTimer.h>

#include "instanceManager.h"

using namespace std;


DEFINE_TYPEHANDLE(InstanceManager)
DEFINE_TYPEHANDLE(InstancePool)


InstancePool::InstancePool(const string& name):
    Namable(name)
{
    
}

void InstancePool::add_instance(CullTraverser* trav, CullTraverserData& data) {
    
}

void InstancePool::collect_instances(CullTraverser* trav, CullTraverserData& data) {
    
}


InstancePool* InstanceManager::get_instance_pool(TypeHandle type) const {

    InstancePools::const_iterator it = _pools.find(type);

    if (it != _pools.end())
        return it->second;

    return nullptr;    
}


void InstanceManager::add_instance_pool(TypeHandle type, InstancePool* pool) {

    _pools[type] = pool;
}


bool InstanceManager::is_renderable() const {
    return true;    
}

bool InstanceManager::safe_to_combine() const {
    return false;
}

bool InstanceManager::safe_to_flatten() const {
    return false;
}


InstanceManager::InstanceManager(const string& name):
    PandaNode(name)
{
    set_internal_bounds(new OmniBoundingVolume);
}



void InstanceManager::add_for_draw(CullTraverser* trav, CullTraverserData& data) {
    
    static PStatCollector collector("drawing instances");

    PStatTimer timer(collector);

    for (InstancePools::value_type& it : _pools)
        it.second->collect_instances(trav, data);
}







// void InstanceManager::add_layer(const string& name, 
//     Texture::ComponentType type, Texture::Format format)
// {
    
//     Texture* texture = new Texture(name);
//     texture->setup_2d_texture_array(32, 32, max_bullet_textures, type, format);

//     _layers[name] = texture;
// }


// bool InstanceManager::add_texture(const string& material, 
//                                     const string& layer,
//                                     const string& name) 
// {

//     Layers::iterator it = _layers.find(layer);

//     if (it != _layers.end()) {
//         Filename path(TEXTURE_PATH "bullet_holes");
//         path.set_basename(name);

//         Texture* texture = it->second;
//         int slot = _occupancy.get_lowest_off_bit();

        
//     }
//     return false;
// }



// void InstanceManager::add_bullet_hole(const TransformState* transform, float timestamp) {

//     CDWriter cdata(_cycler);
//     PT(BulletInstanceList) instances = cdata->_instances.get_write_pointer();
//     instances->append(transform, timestamp);
// }






// BulletHoleMaterial* InstanceManager::get_material(const string& name) const {

//     Materials::const_iterator it = _materials.find(name);

//     if (it != _materials.end())
//         return it->second;

//     return nullptr;    
// }


// int InstanceManager::acquire_slot() {

//     int slot = _occupancy.get_lowest_off_bit();
//     if (slot != -1)
//         _occupancy.set_bit(slot);

//     return slot;    
// }
