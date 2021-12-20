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

#include <panda3d/cMetaInterval.h>
#include <panda3d/partBundle.h>
#include <panda3d/cIntervalManager.h>
#include <panda3d/asyncTaskManager.h>
#include <panda3d/eventQueue.h>

#include "animSequenceControl.h"
#include "animSequence.h"


using namespace std;

DEFINE_TYPEHANDLE(AnimSequenceControl)
DEFINE_TYPEHANDLE(SequenceInterval)


NameUniquifier AnimSequenceControl::_names;
CallbackTask* AnimSequenceControl::_check_task;
AnimSequenceControl::Controls AnimSequenceControl::_controls;


void AnimSequenceControl::animation_activated() {

    AnimControl::animation_activated();
    initialize();

    if (_interval != nullptr) {
        if (is_playing()) {
            _interval->start(get_full_fframe(), -1.0, get_play_rate());

            if (_it == _controls.end())
                _it = _controls.insert(_controls.end(), this);
        }
    }
}


AnimSequenceControl::Controls::iterator 
AnimSequenceControl::stopped(Controls::iterator it) {

    nassertr(it != _controls.end(), _controls.end())

    _interval->clear_to_initial();
    _it = _controls.end();

    return _controls.erase(it);
}


void AnimSequenceControl::initialize() {

    if (_initialized)
        return;

    _initialized = true;

    AnimBundle* bundle = get_anim();
    double duration = get_duration();

    for (size_t i=0; i<bundle->get_num_children(); ++i) {

        AnimGroup* group = bundle->get_child(i);

        if (!group->is_of_type(AnimSequence::get_class_type()))
            continue;
        
        AnimSequence* anim = DCAST(AnimSequence, group);

        if (_interval == nullptr)
            _interval = new CMetaInterval(_names.add_name(get_name()));

        for (size_t i=0; i<anim->get_num_tags(); ++i) {
            AnimSequence::Tag tag = anim->get_tag(i);

            _interval->add_c_interval(new SequenceInterval(tag, this), 
                                    min(tag._timestamp, duration), 
                                    CMetaInterval::RS_level_begin);
        }
    }
}


AsyncTask::DoneStatus AnimSequenceControl::check_intervals(AsyncTask* task) {

    Controls::iterator it = _controls.begin();

    while (it != _controls.end()) {
        AnimSequenceControl* control = *it;
        
        if (!control->is_playing())
            it = control->stopped(it);
        else
            ++it;
    }
    return AsyncTask::DS_cont;  
}


AnimSequenceControl::AnimSequenceControl(const string& name, PartBundle* part):
    AnimControl(name, part, 1.0f, 0),
    _it(_controls.end()),
    _initialized(false)
{
    AsyncTaskManager* task_mgr = AsyncTaskManager::get_global_ptr();

    if (_check_task == nullptr)
        setup_check_task();
}


void AnimSequenceControl::setup_check_task() {

    nassertv(_check_task == nullptr)

    AsyncTaskManager* mgr = AsyncTaskManager::get_global_ptr();

    _check_task = new CallbackTask(check_intervals, "check-intervals");
    mgr->add(_check_task);
}


AnimSequenceControl::~AnimSequenceControl() {
    
    if (_it != _controls.end())
        _controls.erase(_it);
}


double AnimSequenceControl::get_duration() const {
    
    return (double)get_num_frames() / get_frame_rate();
}


void AnimSequenceControl::set_event_queue(EventQueue* queue) {
    _event_queue = queue;
}

void AnimSequenceControl::set_event_name(const string& name) {
    _event_name = name;
}


void AnimSequenceControl::throw_event(SequenceInterval* interval) {

    if (!_event_name.empty()) {

        EventQueue* queue = _event_queue;

        if (queue == nullptr)
            queue = EventQueue::get_global_event_queue();

        Event* event = new Event(_event_name);
        event->add_parameter(interval);
        queue->queue_event(event);
    }   
}



SequenceInterval::SequenceInterval(const Tag& tag, AnimSequenceControl* control): 
    CInterval(tag._name, 0.1f, true),
    _tag_type(tag._type),
    _control(control)
{
    
}


void SequenceInterval::priv_initialize(double t) {

    CInterval::priv_initialize(t);

    if (PT(AnimSequenceControl) control = _control.lock())
        control->throw_event(this);
}


SequenceInterval::Tag::Type SequenceInterval::get_tag_type() const {
    return _tag_type;
}
