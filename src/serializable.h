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


#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

#include "utils.h"


void generic_read_datagram(TypedWritable* value, DatagramIterator& scan);


class PropertyBase: public Namable, public TypedWritableReferenceCount {

    REGISTER_TYPE("PropertyBase", Namable, TypedWritableReferenceCount)

public:

    PropertyBase(const std::string& name, TypeHandle type);
    virtual ~PropertyBase() = default;

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;

    TypeHandle get_data_type() const;

private:
    TypeHandle _data_type;
};



template<class T>
class Property: public PropertyBase {

    REGISTER_TEMPLATE("Property", Namable, ReferenceCount)

public:
    Property(const std::string& name, const T& defaultv=T());

    INLINE T get_value() const;
    INLINE void set_value(const T& value);

    INLINE operator T() const;
    INLINE Property<T>& operator =(const T& value);

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;

protected:
    T _value;
};



#define PROPERTY(type, name, ...) \
    Property<type>& name = make_property<type>(name __VA_ARGS__);



class Serializable: public TypedWritableReferenceCount {

    REGISTER_TYPE("Serializable", TypedWritableReferenceCount)

public:
    typedef std::map<std::string, PT(PropertyBase)> Properties;

    Serializable() = default;
    virtual ~Serializable() = default;

    const Properties& get_properties() const;

protected:

    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;

    template<class T, class... Args>
    Property<T>& make_property(const std::string& name, Args... args);

    Properties _properties;
};


#include "serializable.T"

#endif // __SERIALIZABLE_H__