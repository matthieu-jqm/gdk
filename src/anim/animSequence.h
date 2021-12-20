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

#ifndef __ANIMSEQUENCE_H__
#define __ANIMSEQUENCE_H__

#include <panda3d/animBundle.h>

#include "utils.h"



class AnimSequence: public AnimGroup {

    REGISTER_TYPE("AnimSequence", AnimGroup)

protected:
    AnimSequence() = default;
    AnimSequence(AnimGroup* parent, const AnimSequence& copy);

public:
    AnimSequence(AnimGroup* parent, const std::string& name);
    virtual ~AnimSequence() = default;

    struct Tag {
        enum Type {T_event, T_sound};

        Type _type;
        std::string _name;
        double _timestamp;

        bool operator <(const Tag& other) const {
            return _timestamp < other._timestamp;
        }
    };

    size_t get_num_tags() const;
    Tag get_tag(size_t id) const;
    Tag get_tag(const std::string& name) const;

    static void register_with_read_factory();

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual AnimGroup* make_copy(AnimGroup* parent) const override;

    typedef pvector<Tag> Tags;
    Tags _tags;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);
};

#endif // __ANIMSEQUENCE_H__