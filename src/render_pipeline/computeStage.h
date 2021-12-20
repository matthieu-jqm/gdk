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

#ifndef __COMPUTESTAGE_H__
#define __COMPUTESTAGE_H__

#include "renderStage.h"
#include "shaderState.h"


class Shader;

class ComputeStage: public RenderStage {

    REGISTER_TYPE("ComputeStage", RenderStage)

public:
    ComputeStage(const std::string& name="compute");
    virtual ~ComputeStage() = default;

    static void register_with_read_factory();

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;
    virtual int complete_pointers(TypedWritable** p_list, BamReader* manager) override;

    virtual void dimensions_changed(LVector2i size) override;

    virtual void setup() override;
    virtual void update() override;

    typedef std::map<std::string, PT(Texture)> Inputs;

    Inputs _inputs;
    LVector2i _group_size;
    LVector2i _num_groups;
    ShaderState _state;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);
};


#endif // __COMPUTESTAGE_H__