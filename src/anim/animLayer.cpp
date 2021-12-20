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

#include "animStack.h"
#include "animLayer.h"


DEFINE_TYPEHANDLE(AnimLayer)


AnimLayer::AnimLayer(const std::string& name): 
    Namable(name),
    _stack(nullptr)
{
    
}


AnimLayer::AnimLayer(const AnimLayer& copy): 
    Namable(copy.get_name()),
    _stack(copy._stack)
{
    if (_stack != nullptr)
        _stack->add_layer(this);
}


CPT(TransformState) AnimLayer::update() {
    
    return TransformState::make_identity();
}


AnimStack* AnimLayer::get_stack() const {
    
    return _stack;
}


void AnimLayer::remove() {

    if (_stack != nullptr)
        _stack->remove_layer(this);
}
