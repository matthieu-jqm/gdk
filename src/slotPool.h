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

#ifndef __SLOTPOOL_H__
#define __SLOTPOOL_H__


#include <stack>
#include <vector>
#include <panda3d/pnotify.h>


template<class T>
class SlotPool {

public:

    struct Slot {
        T _value;
        bool _used;
    };

    typedef std::stack<size_t> UnusedPoolContainer;
    typedef std::vector<Slot> PoolContainer;

    typedef typename PoolContainer::iterator iterator;
    typedef typename PoolContainer::const_iterator const_iterator;


    SlotPool(size_t size=1024);
    ~SlotPool() = default;

    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    size_t acquire_slot(const T& value=T());
    void release_slot(size_t slot);

    void resize(size_t size);

    size_t get_num_items() const;
    size_t get_size() const;

    T at(size_t n) const;

    T& operator[](size_t n);
    const T& operator[](size_t n) const;

    bool is_slot_used(size_t slot) const;
    size_t get_contiguous_size() const;

private:
    UnusedPoolContainer _unused;
    PoolContainer _pool;
    size_t _num_items;
};

#include "slotPool.T"

#endif // __SLOTPOOL_H__