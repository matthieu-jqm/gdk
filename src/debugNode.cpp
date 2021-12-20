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

#include <panda3d/cullTraverser.h>
#include <panda3d/cullTraverserData.h>

#include "debugNode.h"


using namespace std;


void DebugNode::add_for_draw(CullTraverser* trav, CullTraverserData& data) {
    
    MyCullHandler handler(trav->get_cull_handler(), get_state());
    trav->set_cull_handler(&handler);

    BulletDebugNode::add_for_draw(trav, data);

    trav->set_cull_handler(handler._actual_handler);
}


void DebugNode::MyCullHandler::record_object(CullableObject* object, 
                                            const CullTraverser *traverser)
{
    // now apply our custom render state
    object->_state = object->_state->compose(_state);
    _actual_handler->record_object(object, traverser);
}


DebugNode::MyCullHandler::MyCullHandler(CullHandler* actual_handler, 
                                        const RenderState* state):
    _actual_handler(actual_handler),
    _state(state)
{
    
}


DebugNode::DebugNode(const string& name):
    BulletDebugNode(name.c_str())
{
    
}
