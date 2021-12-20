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


template<class T, ShortSizePoT PageSize>
TypeHandler<T, PageSize>::TypeHandler() {

    ensure_up_to_date();
}


template<class T, ShortSizePoT PageSize>
void TypeHandler<T, PageSize>::set_handler(TypeHandle type, T handler, bool sub) {
    
    if (sub)
        set_handler(type, handler, recursive());
    else
        set_handler(type, handler, normal());
}


template<class T, ShortSizePoT PageSize>
void TypeHandler<T, PageSize>::set_handler(TypeHandle type, T handler, recursive) {
    
    set_handler(type, handler, normal());

    int num_child = type.get_num_child_classes();

    for (int i=0; i<num_child; ++i) {
        TypeHandle subtype = type.get_child_class(i);
        set_handler(subtype, handler, recursive());
    }
}


template<class T, ShortSizePoT PageSize>
void TypeHandler<T, PageSize>::set_handler(TypeHandle type, T handler, normal) {

    int index = type.get_index();
    int table_index = index >> PageSize;
    int page_offset = index & (page_size - 1);

    Page*& page = _page_table[table_index];

    if (page == nullptr)
        page = new Page;

    std::swap(handler, page->at(page_offset));
}


template<class T, ShortSizePoT PageSize>
T TypeHandler<T, PageSize>::get_handler(TypeHandle type) const {

    int index = type.get_index();
    int table_index = index >> PageSize;
    int page_offset = index & (page_size - 1);

    nassertr(table_index < _page_table.size(), T())

    Page* page = _page_table[table_index];

    if (page != nullptr)
        return page->at(page_offset);

    return T();
}


template<class T, ShortSizePoT PageSize>
void TypeHandler<T, PageSize>::ensure_up_to_date() const {
    
    TypeRegistry* registery = TypeRegistry::ptr();
    int num_types = registery->get_num_typehandles();

    if (UNLIKELY((_page_table.size() << PageSize) < num_types))
        _page_table.resize(get_ceil_size(num_types, PageSize));
}


template<class T, ShortSizePoT PageSize>
TypeHandler<T, PageSize>::~TypeHandler() {
    
    for (Page* page: _page_table) {
        if (page != nullptr)
            delete page;
    }
}       

