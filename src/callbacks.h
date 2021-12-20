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

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <panda3d/asyncTask.h>
#include <functional>
#include "utils.h"


#define type_of(value) std::remove_reference<decltype(value)>::type
#define type_of_this type_of(*this)

#define method_addr(method) &type_of_this::method

/**
 * @def as_callback(method)
 * encapsule un méthode pour être utilisé comme pointeur sur fonction.
*/
#define as_function(method) &Callback<decltype(method), method>::call

/**
 * @def callback(method)
 * facilite le passage d'un callback à une fonction attendant en paramètres un
 * pointeur sur fonction et un pointeur sur une donnée qui sera passé au
 * callback lors de son appel
 * @code do_later(callback(my_method)); <-> do_later(my_method, this);
 * @remark cette macro doit être utilisé à l'intérieur de la classe dont 
 * la méthode appartient
*/
#define callback(method) as_function(method_addr(method)), this

/**
 * @def method(function)
 * créer un callback sur une methode
 * @remark cette macro doit être utilisé à l'intérieur de la classe dont 
 * la méthode appartient
*/
#define method(function) std::bind(std::mem_fn(method_addr(function)), \
                                    this, std::placeholders::_1)


#define named_method(function) method(function), #function
#define named_task(function) function, #function



template <class T, T> class Callback;

/**
 * @brief classe statique servant d'interface sur un méthode 
 * membre d'un objet
 * @code
 * auto* fn_ptr = &Callback<decltype(&cls::method), &cls::method>::call;
 * fn_ptr() 
 * @details voir la macro callback() pour simplifier l'utilisation 
 * de cette classe
*/
template<class T, class R, class ...A, R (T::*func)(A...)>
class Callback<R (T::*)(A...), func> {

public:
    static R call(A... args, void* data);
};

/**
 * @brief classe de tâche générique servant à appeler un object appelable
*/
class CallbackTask: public AsyncTask {

    REGISTER_TYPE("CallbackTask", AsyncTask)

public:
    ALLOC_DELETED_CHAIN(CallbackTask);

    using Func = std::function<DoneStatus (AsyncTask*)>;

    virtual DoneStatus do_task();

    Func get_callback() const;
    void set_callback(const Func& func);

    CallbackTask(const Func& callback, const std::string& name="callback");
    CallbackTask(const std::string& name="callback");
    virtual ~CallbackTask() = default;

private:
    Func _callback;
};


class TaskPool {

protected:
    TaskPool();
    ~TaskPool();

    void register_task(AsyncTask* task, const std::string& name, int sort=0);
    
    size_t unregister_task(const std::string& name);
    bool unregister_task(AsyncTask* task);

    size_t get_num_tasks() const;
    AsyncTask* get_task(size_t id) const;


    AsyncTask* do_task_later(const CallbackTask::Func& callback, 
                        const std::string& name, float delay, int sort=0);


    AsyncTask* do_add_task(const CallbackTask::Func& callback, 
                        const std::string& name, int sort=0);


    template<class D>
    AsyncTask* do_task_later(const CallbackTask::Func& callback, 
                    const std::string& name, float delay, int sort=0, D data=D());

    template<class D>
    AsyncTask* do_add_task(const CallbackTask::Func& callback, 
                        const std::string& name, int sort=0, D data=D());

private:
    TaskPool(const TaskPool&) = delete;
    TaskPool& operator =(const TaskPool&) = delete;

    typedef pset<PT(AsyncTask)> Tasks;
    Tasks _tasks;

    PT(AsyncTaskManager) _mgr;
};

#include "callbacks.T"

#endif // __CALLBACKS_H__