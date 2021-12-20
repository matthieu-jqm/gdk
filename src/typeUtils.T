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
T TypeUtils<T>::find(const Data& data, TypeHandle type, MatchType match, T def) {

    switch (match) {
        case MT_best: 
            return find_best(data, type, def);
        case MT_first: 
            return find_first(data, type, def);
        case MT_exact: 
            return find_exact(data, type, def);
    }
    return def;
}


template<class T>
T TypeUtils<T>::find_best(const Data& data, TypeHandle type, T def) {
    
    std::set<int> types_index;

    for (const auto& entry : data) 
        types_index.insert(entry.first.get_index());

    int index = type.get_best_parent_from_Set(types_index);

    if (index != -1) {
        TypeRegistry* registry = TypeRegistry::ptr();
        TypeHandle match_type = registry->find_type_by_id(index);
        return data.find(match_type)->second;
    }
    return def;
}


template<class T>
T TypeUtils<T>::find_exact(const Data& data, TypeHandle type, T def) {

    typename Data::const_iterator it = data.find(type);

    if (it != data.end())
        return it->second;

    return def;
}


template<class T>
T TypeUtils<T>::find_first(const Data& source, TypeHandle type, T def) {

    for (const std::pair<TypeHandle, T>& entry : source) {
        TypeHandle found = entry.first;
        if (found == type || type.is_derived_from(found))
            return entry.second;
    }
    return def;
}