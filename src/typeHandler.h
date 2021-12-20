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

#ifndef __TYPEHANDLER_H__
#define __TYPEHANDLER_H__


#include <panda3d/typeHandle.h>
#include "utils.h"


/*** @brief 
 * This class provide an assotiative container with constant time
 * access for type's handers. Internally this class use a PageTable
 * where each page is fixed size array that store the actual data.
 * This is because the type's ids stored in are generally sparse
 * and use a lot of memory uselessly.
 */
template<class T, ShortSizePoT PageSize=SST_64>
class TypeHandler: public std::set<TypeHandle> {

public:
    // setter mode for tag dispatch
    struct normal {};
    struct recursive {};

    TypeHandler();
    ~TypeHandler();

    void set_handler(TypeHandle type, T handler, bool sub=false);
    void set_handler(TypeHandle type, T handler, recursive);
    void set_handler(TypeHandle type, T handler, normal);

    T get_handler(TypeHandle type) const;
    void ensure_up_to_date() const;

    enum { page_size = 0x1 << PageSize };

private:

    typedef std::array<T, page_size> Page;
    typedef std::vector<Page*> PageTable;

    mutable PageTable _page_table;
};

#include "typeHandler.T"

#endif // __TYPEHANDLER_H__