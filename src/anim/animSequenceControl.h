// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// Among Z is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Among Z is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with Among Z. If not, see <http://www.gnu.org/licenses/>.

#ifndef __ANIMSEQUENCECONTROL_H__
#define __ANIMSEQUENCECONTROL_H__

#include <panda3d/animControl.h>
#include <panda3d/cInterval.h>
#include <panda3d/nameUniquifier.h>

#include "animSequence.h"
#include "callbacks.h"

class AnimSequence;
class CMetaInterval;
class AnimSequenceControl;


class SequenceInterval: public CInterval {

    REGISTER_TYPE("SequenceInterval", CInterval)

public:
    using Tag = AnimSequence::Tag;

    SequenceInterval(const Tag& tag, AnimSequenceControl* control);
    ~SequenceInterval() = default;

    Tag::Type get_tag_type() const;

private:
    virtual void priv_initialize(double t) override;

    WPT(AnimSequenceControl) _control;
    Tag::Type _tag_type;
};



class AnimSequenceControl: public AnimControl {

    REGISTER_TYPE("AnimSequenceControl", AnimControl)

public:
    AnimSequenceControl(const std::string& name, PartBundle* part);
    ~AnimSequenceControl();

    double get_duration() const;

    void set_event_queue(EventQueue* queue);
    void set_event_name(const std::string& name);

    void throw_event(SequenceInterval* interval);

protected:
    virtual void animation_activated();

private:
    static void setup_check_task();
    static CallbackTask* _check_task;

    EventQueue* _event_queue;
    std::string _event_name;

    PT(CMetaInterval) _interval;

    void initialize();
    static AsyncTask::DoneStatus check_intervals(AsyncTask* task);

    bool _initialized;
    
    typedef plist<PT(AnimSequenceControl)> Controls;
    static Controls _controls;

    Controls::iterator stopped(Controls::iterator it);

    Controls::iterator _it;

    static NameUniquifier _names;
};

#endif // __ANIMSEQUENCECONTROL_H__