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

#ifndef __TYPEUTILS_H__
#define __TYPEUTILS_H__

#include <panda3d/typeHandle.h>


template<class T>
class TypeUtils {

    using Data = pmap<TypeHandle, T>;

public:

    enum MatchType{
        MT_best,
        MT_first,
        MT_exact
    };

    static T find(const Data& data, TypeHandle type, MatchType match, T def=T());

    static T find_best(const Data& data, TypeHandle type, T def=T());
    static T find_exact(const Data& data, TypeHandle type, T def=T());
    static T find_first(const Data& source, TypeHandle type, T def=T());
};

#include <typeUtils.T>

#endif // __TYPEUTILS_H__