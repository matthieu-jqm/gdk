// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// This file is part of projet.
// 
// projet is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// projet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with projet.  If not, see <http://www.gnu.org/licenses/>.

#include <panda3d/asyncTaskManager.h>
#include <panda3d/bamFile.h>

#include "config_gdk.h"
#include "callbacks.h"
#include "utils.h"


using namespace std;


size_t unregister_task(const string& name, void* data) {

	AsyncTaskManager* task_mgr = AsyncTaskManager::get_global_ptr();
    size_t count = 0;

    AsyncTaskCollection tasks = task_mgr->find_tasks(name);

    for (int i=0; i<tasks.get_num_tasks(); ++i) {
        AsyncTask* task = tasks.get_task(i);
            
        if (data != nullptr) {

            if (task->is_of_type(GenericAsyncTask::get_class_type())) {
                if (((GenericAsyncTask*)task)->get_user_data() != data)
                    continue;
            } else
                continue;
        }
        task_mgr->remove(task);
        ++count;
    }

    return count;
}


bool register_hook(EventHandler::EventCallbackFunction* func, 
                void* data, const string& name)
{    
    EventHandler* handler = EventHandler::get_global_event_handler();
    return handler->add_hook(name, func, data);
}


PandaNode* find_parent(PandaNode* node, TypeHandle type) {
    
    while (node->get_num_parents()) {
        node = node->get_parent(0);

        if (node->is_of_type(type))
            return node;
    }
    return nullptr;
}


RefHolder::RefHolder(ReferenceCount* object):
    _object(object)
{
    if (object != nullptr)
        object->ref();
}

RefHolder::~RefHolder() {
    
    if (_object != nullptr)
        _object->unref();
}



TypedWritable* read_object(Filename filename) {
    
    BamFile file;
    TypedWritable* object = nullptr;

    if (file.open_read(filename)) {
        object = file.read_object();

        if (object == nullptr)
            gdk_error("failed to read object from " << filename);

        else if (!file.resolve())
            gdk_error("failed to resolve object from " << filename);

        RefHolder holder(object->as_reference_count());
        file.close();

    } else
        gdk_error("could not open file " << filename);
    
    return object;
}


bool write_object(Filename filename, const TypedWritable* object) {

    BamFile file;
    bool success = false;

    if (file.open_write(filename)) {

        if (!file.write_object(object))
            gdk_error("could not write object to " << filename);
        else {
            gdk_info("written object to " << filename);
            success = true;
            file.close();
        }
    } else
        gdk_error("could not open file " << filename);

    return success;
}



string get_unique_name(const string& name, pvector<string> names) {

    string new_name = name;
    int i = 0;

    while (find(names.begin(), names.end(), new_name) != names.end()) {
        stringstream ss;
        ss << name << '.' << setfill('0') << setw(4) << i++;
        new_name = ss.str();
    };
    return new_name;
}