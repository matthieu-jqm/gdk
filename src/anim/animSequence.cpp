// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// Among Z is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Among Z is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with Among Z. If not, see <http://www.gnu.org/licenses/>.

#include <panda3d/animBundle.h>
#include "animSequence.h"


using namespace std;

DEFINE_TYPEHANDLE(AnimSequence)



void AnimSequence::write_datagram(BamWriter* manager, Datagram& me) {

    AnimGroup::write_datagram(manager, me);

    size_t num_tags = _tags.size();

    me.add_uint16(num_tags);

    for (size_t i=0; i<num_tags; ++i) {
        Tag& tag = _tags[i];
        me.add_uint8(tag._type);
        me.add_float64(tag._timestamp);
        me.add_string(tag._name);
    }
}


void AnimSequence::fillin(DatagramIterator& scan, BamReader* manager) {
    
    AnimGroup::fillin(scan, manager);

    size_t num_tags = scan.get_uint16();

    for (size_t i=0; i<num_tags; ++i) {
        Tag tag;
        tag._type = static_cast<Tag::Type>(scan.get_uint8());
        tag._timestamp = scan.get_float64();
        tag._name = scan.get_string();
        _tags.push_back(tag);
    }

    sort(_tags.begin(), _tags.end());
}


TypedWritable* AnimSequence::make_from_bam(const FactoryParams& params) {
    
    AnimSequence* me = new AnimSequence;
    DatagramIterator scan;
    BamReader *manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);
    return me;
}


AnimGroup* AnimSequence::make_copy(AnimGroup* parent) const {
    
    return new AnimSequence(parent, *this);
}


size_t AnimSequence::get_num_tags() const {
    
    return _tags.size();
}


AnimSequence::Tag AnimSequence::get_tag(size_t id) const {
    
    nassertr(id < _tags.size(), Tag())
    return _tags[id];
}


AnimSequence::Tag AnimSequence::get_tag(const string& name) const {
    
    for (const Tag& tag: _tags) {
        if (tag._name == name)
            return tag;
    }
    return Tag();
}


void AnimSequence::register_with_read_factory() {
    
    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}


AnimSequence::AnimSequence(AnimGroup* parent, const AnimSequence& copy): 
    AnimGroup(parent, copy),
    _tags(copy._tags)
{
    
}


AnimSequence::AnimSequence(AnimGroup* parent, const string& name):
    AnimGroup(parent, name)
{
    
}
