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

#ifndef __COLLISIONHANDLER_H__
#define __COLLISIONHANDLER_H__


#include <panda3d/callbackObject.h>
#include "typeHandler.h"
#include "utils.h"

class BulletPersistentManifold;
class PandaNode;
class BulletWorld;


class CollisionHandler: public CallbackObject {

public:
    ALLOC_DELETED_CHAIN(CollisionHandler)

    CollisionHandler(BulletWorld* world);
    ~CollisionHandler() = default;

    virtual void do_callback(CallbackData *cbdata);

    typedef void (*CollideCallback)(BulletPersistentManifold*, 
                                PandaNode*, PandaNode*);

    static void register_handler(TypeHandle type, CollideCallback handler);
    static void unregister_handler(TypeHandle type);

private:
    void dispatch(BulletPersistentManifold* manifold, 
                PandaNode* node, PandaNode* other);

    static TypeHandler<CollideCallback> _handlers;

private:
    BulletWorld* _world;
};


#endif // __COLLISIONHANDLER_H__