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

#ifndef __COLLISIONDETECTION_H__
#define __COLLISIONDETECTION_H__

#include <panda3d/register_type.h>
#include <panda3d/bulletSoftBodyNode.h>
#include <panda3d/bulletRigidBodyNode.h>

#include "collisionHandler.h"
#include "utils.h"


class PandaNode;


template<class T>
class CollisionDetection: public T {

    REGISTER_TEMPLATE("CollisionDetection", T)

public:
    using Type = CollisionDetection<T>;
    
    template <class... Args>
    CollisionDetection(Args&&... args);
    virtual ~CollisionDetection() = default;

    virtual void do_callback(BulletPersistentManifold* manifold,
                            PandaNode* other);

    static void post_init();

private:
    static void handler(BulletPersistentManifold* manifold, 
                        PandaNode* object, PandaNode* other);
};

#include "collisionDetection.T"

#endif // __COLLISIONDETECTION_H__