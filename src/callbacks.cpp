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

#include <panda3d/asyncTaskManager.h>
#include "callbacks.h"


using namespace std;

DEFINE_TYPEHANDLE(CallbackTask)


size_t TaskPool::unregister_task(const string& name) {

    Tasks::iterator it;
    vector<Tasks::iterator> to_remove;

    for (it = _tasks.begin(); it != _tasks.end(); ++it) {
        AsyncTask* task = *it;
        if (task->get_name() == name) {
            _mgr->remove(task);
            to_remove.push_back(it);
        }
    }
    size_t num_erased = to_remove.size();

    for (Tasks::iterator it: to_remove)
        _tasks.erase(it);
    
    return num_erased;
}


bool TaskPool::unregister_task(AsyncTask* task) {
    
    Tasks::iterator found =  _tasks.find(task);
   
    if (found != _tasks.end()) {
        _tasks.erase(found);
        return true;
    }
    return false;
}


size_t TaskPool::get_num_tasks() const {
    
    return _tasks.size();
}


AsyncTask* TaskPool::get_task(size_t id) const {
    
    nassertr(id < _tasks.size(), nullptr)
    return *next(_tasks.begin(), id);
}


AsyncTask* TaskPool::do_task_later(const CallbackTask::Func& callback, 
                            const string& name, float delay, int sort) 
{

    AsyncTask* task = new CallbackTask(callback);
    task->set_delay(delay);
    register_task(task, name, sort);

    return task;
}


void TaskPool::register_task(AsyncTask* task, const string& name, int sort) {

    task->set_name(name);
	task->set_sort(sort);

    bool inserted = _tasks.insert(task).second;
    nassertv(inserted)

	_mgr->add(task);
}


AsyncTask* TaskPool::do_add_task(const CallbackTask::Func& callback, 
                                const string& name, int sort)
{
            
    AsyncTask* task = new CallbackTask(callback);
    register_task(task, name, sort);

    return task;
}


TaskPool::TaskPool():
    _mgr(AsyncTaskManager::get_global_ptr())
{
    
}


TaskPool::~TaskPool() {
    
    Tasks::iterator it = _tasks.begin();

    for (; it != _tasks.end(); ++it)
        _mgr->remove(*it);
}


AsyncTask::DoneStatus CallbackTask::do_task() {

    if (_callback)
        return _callback(this);
    
    return AsyncTask::DS_done;
}


CallbackTask::Func CallbackTask::get_callback() const {
    
    return _callback;
}


void CallbackTask::set_callback(const Func& func) {
    
    _callback = func;
}


CallbackTask::CallbackTask(const Func& callback, const string& name):
    AsyncTask(name), _callback(callback) 
{
    
}


CallbackTask::CallbackTask(const string& name): AsyncTask(name) {
    
}    

