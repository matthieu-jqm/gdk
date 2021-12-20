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


#ifndef __LIGHTBASE_H__
#define __LIGHTBASE_H__


#include <panda3d/lightLensNode.h>

#include "cullHandlerHook.h"
#include "utils.h"


class LightAttribs;
class CallbackObject;

class LightBase: public LightLensNode {

    REGISTER_TYPE("LightBase", LightLensNode)

public:
    virtual ~LightBase();

    virtual void add_for_draw(CullTraverser* trav, CullTraverserData& data);
    virtual bool is_renderable() const override;
    virtual bool safe_to_combine() const override;
    virtual bool safe_to_flatten() const override;

    virtual void bind(GraphicsStateGuardianBase* gsg, 
                    const NodePath& light, int light_id);

    void set_shadow_caster(bool caster);

    float get_energy() const;
    void set_energy(float energy);

    int get_shadow_slot() const;
    void set_shadow_slot(int slot);

protected:
    LightBase(const std::string& name, Lens* lens);
    LightBase(const LightBase& copy);
    
    virtual void fillin(DatagramIterator &scan, BamReader *manager) override;
    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;

    virtual void setup_shadow_map() override;

private:
    class CData : public CycleData {
    public:
        CData();
        CData(const CData& copy);

        virtual CycleData* make_copy() const override;
        virtual void write_datagram(BamWriter *manager, Datagram &dg) const override;
        virtual void fillin(DatagramIterator &scan, BamReader *manager) override;
        TypeHandle get_parent_type() const override {
            return LightBase::get_class_type();
        }
    
        int _shadow_slot;
        float _energy;
    };


    PipelineCycler<CData> _cycler;
    typedef CycleDataReader<CData> CDReader;
    typedef CycleDataWriter<CData> CDWriter;
};


#endif // __LIGHTBASE_H__