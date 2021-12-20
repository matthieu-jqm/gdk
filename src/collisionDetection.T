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

#include "config_gdk.h"


template<class T>
DEFINE_TYPEHANDLE(CollisionDetection<T>)


template<class T>
template<class... Args>
CollisionDetection<T>::CollisionDetection(Args&&... args):
    T(std::forward<Args>(args)...)
{}

template<class T>
void CollisionDetection<T>::do_callback(BulletPersistentManifold* manifold,
                                        PandaNode* other) 
{
    gdk_warning("collision callback not implemented for " << get_type());
}


template<class T>
void CollisionDetection<T>::post_init() {
    CollisionHandler::register_handler(_type_handle, handler);
}


template<class T>
void CollisionDetection<T>::handler(BulletPersistentManifold* manifold, 
                                    PandaNode* object, PandaNode* other)
{
    static_cast<Type*>(object)->do_callback(manifold, other);
}