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

template<class T>
BitArray InstanceIndex<T>::_indices;

template<class T>
LightMutex InstanceIndex<T>::_lock;

template<class T>
DEFINE_TYPEHANDLE(LocalNodePath<T>)


template<class T>
InstanceIndex<T>::InstanceIndex() {

    LightMutexHolder holder(_lock);

    _index = _indices.get_lowest_off_bit();

    if (_index == -1)
        _index = _indices.get_num_bits();
    
    _indices.set_bit(_index);
}


template<class T>
InstanceIndex<T>::~InstanceIndex() {

    LightMutexHolder holder(_lock);
    _indices.clear_bit(_index);
}

template<class T>
InstanceIndex<T>::operator int() const {
    return _index;
}



template<class T>
bool HandlerCache<T>::has_handler(size_t index) const {
    
    if (index < _handlers.size()) {
        if (_handlers[index].is_valid_pointer())
            return true;
    }
    return false;
}


template<class T>
void HandlerCache<T>::set_handler(size_t index, T* pointer) {
    
    if (index >= _handlers.size())
        _handlers.resize(index + 1);
    
    _handlers[index] = pointer;
}


template<class T>
PT(T) HandlerCache<T>::lock(size_t index) const {
    
    if (index < _handlers.size())
        return _handlers[index].lock();
    
    return nullptr;
}


template<class T>
template<class... Args>
LocalNodePath<T>::LocalNodePath(Args&&... args):
    T(std::forward<Args>(args)...), 
    LocalRef<NodePath>(this, this) 
{

}


template<class T>
Singleton<T>::Singleton(T* ptr):
    std::unique_ptr<T>(ptr)
{}

template<class T>
INLINE Singleton<T>& Singleton<T>::operator =(T* ptr) {
    std::unique_ptr<T>::reset(ptr);
    return *this;
}

template<class T>
INLINE Singleton<T>::operator T*() const {
    return std::unique_ptr<T>::get();
}