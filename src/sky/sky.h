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


#ifndef __SKY_H__
#define __SKY_H__

#include <panda3d/pandaNode.h>

#include "component.h"
#include "color.h"

class DirectionalLightNode;
class Scene;


class Sky: public Component {

    REGISTER_TYPE("Sky", Component)

public:
    Sky() = default;
    virtual ~Sky() = default;

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;

    virtual void render(RenderStage* stage) override;
    virtual void update() override;

private:
    static TypedWritable *make_from_bam(const FactoryParams& params);

    WPT(Scene) _scene;


    class CData : public CycleData {
    public:
        CData();
        CData(const CData& copy);

        virtual CycleData* make_copy() const override;
        virtual void write_datagram(BamWriter* manager, Datagram& dg) const override;
        virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
        TypeHandle get_parent_type() const override {
            return Sky::get_class_type();
        }
    
        float _atmosphere_density;
        float _atmosphere_height;
        float _atmosphere_intensity;

        Color _atmosphere_color;
        Color _atmosphere_inscattering;
        Color _atmosphere_extinction;

        float _mie_scattering;
        float _mie_anisotropy;

        float _distance_scale;
        float _atmosphere_falloff;
        float _radiance_gamma;

        Color _ground_albedo;
        float _ground_offset;
    };

    PipelineCycler<CData> _cycler;
    typedef CycleDataReader<CData> CDReader;
    typedef CycleDataWriter<CData> CDWriter;

    PTA_LVecBase3f _light_color;
    PTA_LVecBase3f _atmosphere_color;
    PTA_LVecBase3f _atmosphere_inscattering;
    PTA_LVecBase3f _atmosphere_extinction;
    PTA_LVecBase3f _ground_albedo;

    PTA_float _angular_diameter;
    PTA_float _atmosphere_density;
    PTA_float _atmosphere_height;
    PTA_float _atmosphere_intensity;

    PTA_float _mie_scattering;
    PTA_float _mie_anisotropy;

    PTA_float _distance_scale;
    PTA_float _atmosphere_falloff;
    PTA_float _radiance_gamma;

    PTA_float _ground_offset;
};

#endif // __SKY_H__