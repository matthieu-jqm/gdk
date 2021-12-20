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

#ifndef __DOWNSCALEDEPTH_H__
#define __DOWNSCALEDEPTH_H__

#include "renderStage.h"
#include "shaderState.h"

extern ConfigVariableInt num_extra_depth_lods;


class DownscaleDepth: public RenderStage {

    REGISTER_TYPE("DownscaleDepth", RenderStage)

public:
    DownscaleDepth();
    virtual ~DownscaleDepth() = default;

    static void register_with_read_factory();

protected:
    virtual void dimensions_changed(LVector2i size) override;

    virtual void setup() override;
    virtual void update() override;

    void write_datagram(BamWriter* manager, Datagram& me);
    void fillin(DatagramIterator& scan, BamReader* manager);
    int complete_pointers(TypedWritable** p_list, BamReader* reader);

    PT(Texture) _linear_depth;
    PT(Texture) _depth;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);

    LVecBase2i _groups;
    PTA_LVecBase2 _texel_size;

    ShaderState _linearize;
    ShaderState _downscale;
};

#endif // __DOWNSCALEDEPTH_H__