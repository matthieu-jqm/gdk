
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

#include <panda3d/partBundle.h>
#include "partFilterAnim.h"

using namespace std;


PartFilterAnim::PartFilterAnim(PartBundle* bundle): 
    _bundle(bundle),
    _default_stale(true)
{
    
}


void PartFilterAnim::add_filter(const std::string& name) {

    PartSubset& subset = _subsets[name];
    subset.add_include_joint(GlobPattern(name));
    subset.add_exclude_joint(GlobPattern("*"));

    _default_stale = true;
}


const PartSubset& PartFilterAnim::get_filter(const std::string& name) const {
    
    Subsets::const_iterator found = _subsets.find(name);

    if (found != _subsets.end())
        return found->second;

    if (_default_stale)
        init_default();

    return _default;
}


void PartFilterAnim::init_default() const {

    _default = PartSubset();
    _default.add_include_joint(GlobPattern(_bundle->get_name()));

    for (const pair<string, PartSubset>& filter: _subsets) {

        PartGroup* group = _bundle->find_child(filter.first);
        
        if (group == nullptr)
            continue;
        
        _default.add_exclude_joint(filter.first);

        int num_children = group->get_num_children();

         for (int i=0; i<num_children; ++i) {
            PartGroup* child = group->get_child(i);
            string child_name = child->get_name();

            if (_subsets.find(child_name) == _subsets.end())
                _default.add_include_joint(child_name);
        }
    }

    _default_stale = false;
}
