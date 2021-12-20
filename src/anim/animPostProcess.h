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


#ifndef __ANIMPOSTPROCESS_H__
#define __ANIMPOSTPROCESS_H__

#include <panda3d/characterJoint.h>

#include "animLayer.h"
#include "utils.h"


class AnimControl;
class AnimChannelMatrixDynamic;

class AnimPostProcess: public AnimLayer {

    REGISTER_TYPE("AnimPostProcess", TypedReferenceCount)

public:
    AnimPostProcess(const std::string& name);
    virtual ~AnimPostProcess() = default;
    
    bool set_control(AnimControl* control);
    AnimControl* get_control() const;

    CPT(TransformState) get_blend_value() const;
    CPT(TransformState) get_value(int frame) const;

protected:
    typedef CharacterJoint::ChannelType ChannelType;

    PT(AnimControl) _control;
    PT(ChannelType) _channel;

private:
    LMatrix4 _pose;
};
#endif // __ANIMPOSTPROCESS_H__