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
SlotPool<T>::SlotPool(size_t size):
    _num_items(0)
{
    _pool.resize(size);
}

template<class T>
typename SlotPool<T>::const_iterator SlotPool<T>::begin() const {
    return const_iterator(_pool.begin());
}

template<class T>
typename SlotPool<T>::const_iterator SlotPool<T>::end() const {
    return const_iterator(_pool.begin() + _num_items + _unused.size() + 1);
}

template<class T>
typename SlotPool<T>::iterator SlotPool<T>::begin() {
    return iterator(_pool.begin());
}

template<class T>
typename SlotPool<T>::iterator SlotPool<T>::end() {
    return iterator(_pool.begin() + _num_items + _unused.size() + 1);
}


template<class T>
size_t SlotPool<T>::acquire_slot(const T& value) {
    nassertr(_num_items < _pool.size(), 0)
    
    size_t index;
    if (!_unused.empty()) {
        index = _unused.top();
        _unused.pop();
    } else
        index = _num_items;

    _pool[index]._value = value;
    _pool[index]._used = true;

    ++_num_items;
    return index;
}

template<class T>
void SlotPool<T>::release_slot(size_t slot) {
    nassertv(slot < _pool.size());

    _pool[slot]._used = false;
    _unused.push(slot);
    _num_items--;
}


template<class T>
void SlotPool<T>::resize(size_t size) {
    
    if (_pool.size() > size) {

        UnusedPoolContainer new_unused;
        size_t num_unused = _unused.size();

        while (!_unused.empty()) {
            int index = _unused.top();
            if (index < size)
                new_unused.push(index);
            _unused.pop();
        }
        _num_items = std::min(_num_items, size - new_unused.size());
        _unused = std::move(new_unused);
    }

    _pool.resize(size);
}

template<class T>
size_t SlotPool<T>::get_num_items() const {
    return _num_items;
}

template<class T>
T SlotPool<T>::at(size_t n) const {
    return _pool[n]._value;
}

template<class T>
T& SlotPool<T>::operator[](size_t n) {
    return _pool[n]._value;
}

template<class T>
const T& SlotPool<T>::operator[](size_t n) const {
    return _pool[n]._value;
}

template<class T>
size_t SlotPool<T>::get_size() const {
    return _pool.size();
}

template<class T>
size_t SlotPool<T>::get_contiguous_size() const {
    return _num_items + _unused.size();
}

template<class T>
bool SlotPool<T>::is_slot_used(size_t slot) const {
    nassertr(slot < _pool.size(), false)
    return _pool[slot]._used;
}
