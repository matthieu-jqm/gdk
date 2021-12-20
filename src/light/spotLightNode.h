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


#ifndef __SPOTLIGHTNODE_H__
#define __SPOTLIGHTNODE_H__


#include "lightBase.h"


class SpotLightNode: public LightBase {

    REGISTER_TYPE("SpotLightNode", LightBase)

public:
    SpotLightNode(const std::string& name);
    SpotLightNode(const SpotLightNode& copy);
    virtual ~SpotLightNode() = default;

    float get_max_distance() const;
    void set_max_distance(float distance);

    float get_inner_radius() const;
    void set_inner_radius(float radius);

    float get_outter_radius() const;
    void set_outter_radius(float radius);
    
    static void register_with_read_factory();

    virtual int get_class_priority() const override;

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;

private:
    static TypedWritable *make_from_bam(const FactoryParams& params);

    class CData : public CycleData {
    public:
        CData();
        CData(const CData& copy);

        virtual CycleData* make_copy() const override;
        virtual void write_datagram(BamWriter* manager, Datagram& dg) const override;
        virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
        TypeHandle get_parent_type() const override {
            return SpotLightNode::get_class_type();
        }
    
        float _max_distance;
        float _inner_radius;
        float _outter_radius;
    };

    PipelineCycler<CData> _cycler;
    typedef CycleDataReader<CData> CDReader;
    typedef CycleDataWriter<CData> CDWriter;
};

#endif // __SPOTLIGHTNODE_H__