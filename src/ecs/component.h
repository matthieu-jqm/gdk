

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

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <panda3d/typedWritableReferenceCount.h>

#include "utils.h"

class RenderStage;


class Component: public TypedWritableReferenceCount, public Namable {

    REGISTER_TYPE("Component", TypedWritableReferenceCount, Namable)

public:
    Component(const std::string& name="");
    virtual ~Component() = default;

    virtual void render(RenderStage* stage);
    virtual void update();

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
};

#endif // __COMPONENT_H__