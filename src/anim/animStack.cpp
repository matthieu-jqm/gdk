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

#include <panda3d/animChannelMatrixDynamic.h>
#include <panda3d/characterJoint.h>
#include <panda3d/partBundle.h>

#include "animLayer.h"
#include "animStack.h"

using namespace std;


DEFINE_TYPEHANDLE(AnimStack)


AnimStack::AnimStack(const std::string& name): 
    Namable(name),
    _joint(nullptr),
    _bundle(nullptr),
    _output_channel(nullptr)
{
    
}


bool AnimStack::set_bundle(PartBundle* bundle) {
    
    PartGroup* group = bundle->find_child(get_name());
    
    if (group == nullptr)
        return false;

    _joint = DCAST(CharacterJoint, group);
    _bundle = bundle;
    
    // a triky hack here is to create a dummy node so that je joint create 
    // a forced channel which is needed to control the joint.
    // As this is a temporary node, we just instantiate it on the stack.
    PandaNode node("dummy");
    node.local_object();
    
    if (!_joint->apply_control(&node))
        return false;
    
    AnimChannelBase* channel = _joint->get_forced_channel();
    
    if (channel->is_of_type(AnimChannelMatrixDynamic::get_class_type())) {
        _output_channel = DCAST(AnimChannelMatrixDynamic, channel);

        // now, we don't need the dummy node anymore, setting any value
        // to the channel will clear the node pointer.
        _output_channel->set_value(TransformState::make_identity());
        return true;
    }
    _joint->clear_forced_channel();

    return false;
}


void AnimStack::update() {
    
    if (_output_channel.is_null())
        return;

    LMatrix4 mat = _joint->get_default_value();
    CPT(TransformState) net_value = TransformState::make_mat(mat);

    for (AnimLayer* layer: _layers)
        net_value = net_value->compose(layer->update());

    _output_channel->set_value(net_value);
}


void AnimStack::add_layer(AnimLayer* layer) {
    
    // fist we need check if this layer was in annother stack.
    // if so, just remove it from its previous stack.
    if (layer->_stack != nullptr && layer->_stack != this)
        layer->_stack->remove_layer(layer);

    layer->_stack = this;
    _layers.push_back(layer);
}


void AnimStack::insert_layer(AnimLayer* layer, size_t pos) {

    nassertv(pos <= _layers.size())

    if (layer->_stack != nullptr && layer->_stack != this)
        layer->_stack->remove_layer(layer);
    
    layer->_stack = this;
    _layers.insert(next(_layers.begin(), pos), layer);
}


void AnimStack::remove_layer(const std::string& name) {
    
    Layers::iterator  it;

    for (it = _layers.begin(); it != _layers.end(); ++it) {
        if ((*it)->get_name() == name)
            break;
    }
    if (it != _layers.end()) {
        (*it)->_stack = nullptr;
        _layers.erase(it);
    }
}


void AnimStack::remove_layer(AnimLayer* layer) {
    
    Layers::iterator found = find(_layers.begin(), _layers.end(), layer);

    if (found != _layers.end()) {
        (*found)->_stack = nullptr;
        _layers.erase(found);
    }
}


AnimLayer* AnimStack::get_layer(const std::string& name) const {
    
    Layers::const_iterator  it;

    for (it = _layers.begin(); it != _layers.end(); ++it) {
        AnimLayer* layer = *it;
        if (layer->get_name() == name)
            return layer;
    }
    return nullptr;
}


AnimLayer* AnimStack::get_layer(size_t id) const {

    nassertr(id < _layers.size(), nullptr)
    return *next(_layers.begin(), id);
}


size_t AnimStack::get_num_layer() const {
    
    return _layers.size();
}


CharacterJoint* AnimStack::get_joint() const {

    return _joint;    
}


PartBundle* AnimStack::get_part_bundle() const {
    
    return _bundle;
}
