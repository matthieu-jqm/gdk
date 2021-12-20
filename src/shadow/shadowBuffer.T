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

#include <panda3d/displayRegionCullCallbackData.h>

#include "shadowBufferManager.h"


template<LightType Type>
DEFINE_TYPEHANDLE(_ShadowBuffer<Type>)


template<LightType Type>
ShadowBuffer<Type>::ShadowBuffer() {
    
}



template<LightType Type>
void _ShadowBuffer<Type>::post_init(TypeHandle) {
    
    ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();
    mgr->register_buffer(new ShadowBuffer<Type>);
}


template<LightType Type>
_ShadowBuffer<Type>::_ShadowBuffer():
    ShadowBufferBase(get_light_name(Type))
{

}

template<LightType Type>
TypeHandle _ShadowBuffer<Type>::get_light_type() const {
    return LightDataBase<Type>::get_class_type();
}
