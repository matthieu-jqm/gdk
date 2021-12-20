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

#ifndef __ANIMSTACK_H__
#define __ANIMSTACK_H__

#include "utils.h"

class AnimLayer;
class CharacterJoint;
class AnimChannelMatrixDynamic;
class PartBundle;


class AnimStack: public Namable, public TypedReferenceCount {

    REGISTER_TYPE("AnimStack", TypedReferenceCount)

public:
    AnimStack(const std::string& name);
    AnimStack(const AnimStack& copy) = delete;
    virtual ~AnimStack() = default;

    virtual bool set_bundle(PartBundle* bundle);

    virtual void update();

    void add_layer(AnimLayer* layer);
    void insert_layer(AnimLayer* layer, size_t pos);

    void remove_layer(const std::string& name);
    void remove_layer(AnimLayer* layer);
    
    AnimLayer* get_layer(const std::string& name) const;
    AnimLayer* get_layer(size_t id) const;
    
    size_t get_num_layer() const;

    CharacterJoint* get_joint() const;
    PartBundle* get_part_bundle() const;

private:
    typedef plist<PT(AnimLayer)> Layers;
    Layers _layers;

    PT(PartBundle) _bundle;
    PT(CharacterJoint) _joint;
    PT(AnimChannelMatrixDynamic) _output_channel;
};

#endif // __ANIMSTACK_H__