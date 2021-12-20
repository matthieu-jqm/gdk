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

#ifndef __ANIMLAYER_H__
#define __ANIMLAYER_H__

#include <panda3d/namable.h>
#include <panda3d/typedReferenceCount.h>

#include "utils.h"


class AnimStack;


class AnimLayer: public Namable, public TypedReferenceCount {

    REGISTER_TYPE("AnimLayer", Namable, TypedReferenceCount)

public:
    AnimLayer(const std::string& name);
    AnimLayer(const AnimLayer& copy);

    virtual ~AnimLayer() = default;

    virtual CPT(TransformState) update();

    AnimStack* get_stack() const;

    void remove();

private:
    AnimStack* _stack;

    friend class AnimStack;
};


#endif // __ANIMLAYER_H__