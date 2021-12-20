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



template<class T, class R, class ...A, R (T::*func)(A...)>
R Callback<R (T::*)(A...), func>::call(A... args, void* data) {

    T* object = static_cast<T*>(data);
    return (object->*func)(args...);
}



template<class D>
AsyncTask* TaskPool::do_task_later(const CallbackTask::Func& callback, 
                const std::string& name, float delay, int sort, D data) 
{
    auto bound_callback = std::bind(callback, std::placeholders::_1, data);
    return do_task_later(bound_callback, delay, name, sort);
}


template<class D>
AsyncTask* TaskPool::do_add_task(const CallbackTask::Func& callback, 
                    const std::string& name, int sort, D data)
{   
    auto bound_callback = std::bind(callback, std::placeholders::_1, data);
    return do_add_task(bound_callback, name, sort);
}