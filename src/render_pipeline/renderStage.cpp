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

#include <panda3d/graphicsEngine.h>
#include <panda3d/displayRegionCullCallbackData.h>
#include <panda3d/displayRegionDrawCallbackData.h>
#include <panda3d/displayRegion.h>

#include "renderPipeline.h"
#include "renderStage.h"
#include "callbacks.h"

using namespace std;


DEFINE_TYPEHANDLE(RenderStage)



void RenderStage::setup() {
    
}

void RenderStage::release() {
    
}


RenderStage::RenderStage(const RenderStage& copy):
    _pipeline(copy._pipeline),
    _active(copy._active)
{
    if (_pipeline != nullptr)
        _pipeline->add_stage(this);
}


void RenderStage::dimensions_changed(LVector2i size) {
  
}


RenderPipeline* RenderStage::get_pipeline() const {
    return _pipeline;
}

void RenderStage::set_pipeline(RenderPipeline* pipeline) {

    if (pipeline != nullptr)
        _pcollector = PStatCollector(pipeline->get_pcollector(), get_name());

    _pipeline = pipeline;
}


bool RenderStage::is_active() const {
    return _active;
}

void RenderStage::set_active(bool active) {
    _active = active;
}

PStatCollector& RenderStage::get_pcollector() {
    return _pcollector;
}

void RenderStage::write_datagram(BamWriter* manager, Datagram& me) {
    me.add_string(get_name());    
}

void RenderStage::fillin(DatagramIterator& scan, BamReader *manager) {
    set_name(scan.get_string());
}


void RenderStage::update() {
    
}

RenderStage::RenderStage(const string& name):
    Namable(name),
    _pipeline(nullptr),
    _active(true)
{
    
}
