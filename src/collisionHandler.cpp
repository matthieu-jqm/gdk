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

#include <panda3d/bulletContactCallbackData.h>
#include <panda3d/bulletTickCallbackData.h>
#include <panda3d/bulletPersistentManifold.h>
#include <panda3d/bulletWorld.h>

#include "collisionHandler.h"
#include "collisionDetection.h"

using namespace std;


TypeHandler<CollisionHandler::CollideCallback> CollisionHandler::_handlers;


void CollisionHandler::do_callback(CallbackData *cbdata) {

    BulletTickCallbackData* data = DCAST(BulletTickCallbackData, cbdata);
 

    for (int i=0; i<_world->get_num_manifolds(); ++i) {
        BulletPersistentManifold* manifold = _world->get_manifold(i);

        if (manifold == nullptr)
            continue;
            
        PT(PandaNode) node0 = manifold->get_node0();
        PT(PandaNode) node1 = manifold->get_node1();

        int points = manifold->get_num_manifold_points();

        if (points > 0) {
            dispatch(manifold, node0, node1);
            dispatch(manifold, node1, node0);
        }
        manifold->clear_manifold();
        delete manifold;
    }
}


void CollisionHandler::register_handler(TypeHandle type, CollideCallback handler) {
    _handlers.ensure_up_to_date();
    _handlers.set_handler(type, handler, true);
}

void CollisionHandler::unregister_handler(TypeHandle type) {
    _handlers.ensure_up_to_date();
    _handlers.set_handler(type, nullptr, true);
}


void CollisionHandler::dispatch(BulletPersistentManifold* manifold, 
                                PandaNode* node, PandaNode* other) 
{
    TypeHandle type = node->get_type();
    CollideCallback collide_callback = _handlers.get_handler(type);

    if (collide_callback != nullptr)
        collide_callback(manifold, node, other);
}


CollisionHandler::CollisionHandler(BulletWorld* world):
    _world(world)
{
    
}
