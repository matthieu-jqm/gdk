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




template<class T>
DEFINE_TYPEHANDLE(InstancedObject<T>)


template<class T>
template<class... Args>
InstancedObject<T>::InstancedObject(const std::string& name, Args&&... args):
    T(name, std::forward<Args>(args)...),
    _pool(nullptr)
{    

}

template<class T>
bool InstancedObject<T>::is_renderable() const {
    return true;
}

template<class T>
bool InstancedObject<T>::safe_to_combine() const {
    return false;
}

template<class T>
bool InstancedObject<T>::safe_to_flatten() const {
    return false;
}


template<class T>
void InstancedObject<T>::add_for_draw(CullTraverser* trav, CullTraverserData& data) {

    if (UNLIKELY(_pool == nullptr)) {
        InstanceManager* mgr = get_manager();
        if (mgr != nullptr)
            _pool = mgr->get_instance_pool(get_type()); 
    }

    if (_pool != nullptr)
        _pool->add_instance(trav, data);
}


template<class T>
InstanceManager* InstancedObject<T>::get_manager() {

    NodePath np = NodePath::any_path(this);
    np = np.get_top();

    NodePath mgr_np = np.find("**/+InstanceManager");

    if (!mgr_np.is_empty())
        return DCAST(InstanceManager, mgr_np.node());

    return nullptr;
}

















// #include <panda3d/cullHandler.h>


// template<class T>
// CPT(RenderState) InstancePoolSSBO<T>::get_state() const {

//     CPT(ShaderAttrib) sattr = DCAST(ShaderAttrib, 
//         _state->get_attrib(ShaderAttrib::get_class_slot()));

//     ShaderBuffer* buffer = new ShaderBuffer("instance_data", _data, 
//                                             GeomEnums::UH_dynamic);
//     ShaderInput input(_buffer_id, buffer);
//     sattr = DCAST(ShaderAttrib, sattr->set_shader_input(input));


//     if (!sattr->get_flag(ShaderAttrib::F_hardware_instancing))
//         sattr = DCAST(ShaderAttrib, sattr->set_instance_count(_num_instances));

//     return _state->set_attrib(sattr);
// }



// template<class Data>
// InstancePoolSSBO<Data>::InstancePoolSSBO(const std::string& name, 
//                                         const Geom* geom, 
//                                         const RenderState* state, 
//                                         CPT_InternalName id):
//     InstancePool(name, geom, state),
//     _buffer_id(id)

// {
//     nassertv(state->has_attrib(ShaderAttrib::get_class_slot()))
// }


// template<class T>
// void InstancePoolSSBO<T>::add_instance(CullTraverser* trav, CullTraverserData& data) {
    
// }


// template<class T>
// void InstancePoolSSBO<T>::collect_instances(CullTraverser* trav, CullTraverserData& data) {
    
//     if (_instances->)

//     trav->get_cull_handler()->record_object(object, trav);
// }