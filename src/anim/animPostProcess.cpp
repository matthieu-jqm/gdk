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

#include <panda3d/characterJoint.h>
#include <panda3d/animChannelMatrixDynamic.h>
#include "animStack.h"
#include "animPostProcess.h"

using namespace std;

DEFINE_TYPEHANDLE(AnimPostProcess)


bool AnimPostProcess::set_control(AnimControl* control) {

    AnimStack* stack = get_stack();

    nassertr(stack != nullptr, false)
    
    _control = control;
    int index = control->get_channel_index();

    CharacterJoint* joint = stack->get_joint();

    if (joint != nullptr && index < joint->get_max_bound()) {
        AnimChannelBase* channel = joint->get_bound(index);

        _pose = joint->get_default_value();
        _pose.invert_in_place();

        if (channel->is_of_type(ChannelType::get_class_type())) {
            _channel = DCAST(ChannelType, channel);

            return true;
        }
    }
    return false;
}


CPT(TransformState) AnimPostProcess::get_value(int frame) const {
    
    if (_channel.is_null())
        return TransformState::make_identity();
    
    LMatrix4 value;
    _channel->get_value(frame, value);

    return TransformState::make_mat(value);
}


CPT(TransformState) AnimPostProcess::get_blend_value() const {

    AnimStack* stack = get_stack();

    if (_channel.is_null() || _control.is_null() || stack == nullptr)
        return TransformState::make_identity();
    
    LMatrix4 net_value = LMatrix4::zeros_mat();
    LMatrix4 current_value;

    int current_frame = _control->get_frame();
    _channel->get_value(current_frame, current_value);

    PartBundle* bundle = stack->get_part_bundle();

    if (!bundle->get_frame_blend_flag())
        net_value += current_value;

    else {
        int next_frame = _control->get_next_frame();
        double frac = _control->get_frac();

        LMatrix4 next_value;

        _channel->get_value(next_frame, next_value);

        net_value += current_value * (1.0f - frac);
        net_value += next_value * frac;
    }

    return TransformState::make_mat(net_value * _pose);
}


AnimControl* AnimPostProcess::get_control() const {
    
    return _control;
}


AnimPostProcess::AnimPostProcess(const string& name): 
    AnimLayer(name)
{
    
}
