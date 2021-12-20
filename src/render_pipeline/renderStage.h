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

#ifndef __RENDERSTAGE_H__
#define __RENDERSTAGE_H__


#include <panda3d/callbackObject.h>
#include <panda3d/callbackGraphicsWindow.h>
#include <panda3d/nameUniquifier.h>
#include <panda3d/pStatCollector.h>
#include <string>

#include "utils.h"


class DisplayRegion;
class RenderPipeline;
class DisplayRegionDrawCallbackData;
class DisplayRegionCullCallbackData;
class CulledNode;


class RenderStage: public TypedWritableReferenceCount, public Namable {

    REGISTER_TYPE("RenderStage", TypedWritableReferenceCount, Namable)

public:
    RenderStage(const std::string& name="");
    RenderStage(const RenderStage& copy);
    virtual ~RenderStage() = default;

    virtual void dimensions_changed(LVector2i size);

    virtual void setup();
    virtual void release();
    virtual void update();

    RenderPipeline* get_pipeline() const;
    void set_pipeline(RenderPipeline* pipeline);

    bool is_active() const;
    void set_active(bool active);

    PStatCollector& get_pcollector();

protected:
    void write_datagram(BamWriter* manager, Datagram& me);
    void fillin(DatagramIterator& scan, BamReader *manager);

    RenderPipeline* _pipeline;
    bool _active;

    PStatCollector _pcollector;
};

#endif // __RENDERSTAGE_H__