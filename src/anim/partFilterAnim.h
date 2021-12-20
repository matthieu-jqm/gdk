

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

#ifndef __PARTFILTERANIM_H__
#define __PARTFILTERANIM_H__

#include <panda3d/partSubset.h>

class PartBundle;
class PartGroup;


class PartFilterAnim {

public:
    PartFilterAnim(PartBundle* bundle);
    ~PartFilterAnim() = default;

    void add_filter(const std::string& name);
    const PartSubset& get_filter(const std::string& name) const;

private:
    void init_default() const;

    PartBundle* _bundle;

    typedef std::map<std::string, PartSubset> Subsets;
    Subsets _subsets;

    mutable bool _default_stale;
    mutable PartSubset _default;
};

#endif // __PARTFILTERANIM_H__