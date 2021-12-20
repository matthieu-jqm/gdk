
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

#ifndef __DEBUGNODE_H__
#define __DEBUGNODE_H__

#include <panda3d/bulletDebugNode.h>
#include <panda3d/cullHandler.h>


class DebugNode: public BulletDebugNode {

public:
    DebugNode(const std::string& name="debug");
    virtual ~DebugNode() = default;

protected:
    virtual void add_for_draw(CullTraverser* trav, 
                            CullTraverserData& data) override;

    class MyCullHandler: public CullHandler {
    
    public:
        MyCullHandler(CullHandler* actual_handler, const RenderState* state);
        ~MyCullHandler() = default;

        void record_object(CullableObject* object, 
                        const CullTraverser *traverser) override;

        CullHandler* _actual_handler;
        const RenderState* _state;
    };
};

#endif // __DEBUGNODE_H__