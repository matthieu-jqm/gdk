
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

#ifndef __RENDERSCENE_H__
#define __RENDERSCENE_H__

#include "renderStage.h"


class RenderScene: public RenderStage {

    REGISTER_TYPE("RenderScene", RenderStage)

public:
    using RenderTexturePlane = DrawableRegion::RenderTexturePlane;

    RenderScene(const std::string& name="render-scene");
    virtual ~RenderScene() = default;


    virtual void dimensions_changed(LVector2i size) override;

    void add_bin(const std::string& name, 
        CullBinEnums::BinType type=CullBinEnums::BT_unsorted, int sort=0);

    void remove_bin(const std::string& name);

    static void register_with_read_factory();

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& me) override;
    virtual void fillin(DatagramIterator& scan, BamReader *manager) override;
    virtual int complete_pointers(TypedWritable** p_list, BamReader* reader) override;

    bool setup_buffer(LVector2i size);

    virtual void render(Thread* current_thread);
    void draw(DisplayRegion* dr, Thread* current_thread);

    virtual void update() override;
    virtual void release() override;
    virtual void setup() override;

    struct BindingSlot {
        PT(Texture) _target;
        bool _clear;
    };

    typedef std::array<BindingSlot, RenderTexturePlane::RTP_COUNT> BindingSlots;
    typedef std::vector<int> BinIndices;

    PT(GraphicsBuffer) _buffer;
    FrameBufferProperties _fb_props;
    BinIndices _bin_indices;
    BindingSlots _bindings;

private:
    void add_target(RenderTexturePlane);
    static TypedWritable* make_from_bam(const FactoryParams& params);
};

#endif // __RENDERSCENE_H__