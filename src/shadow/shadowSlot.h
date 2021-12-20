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

#ifndef __SHADOWSLOT_H__
#define __SHADOWSLOT_H__

#include <panda3d/displayRegion.h>

#include "graphicsCallback.h"
#include "shaderState.h"
#include "utils.h"

class ShadowBuffer;
class LightBase;
class ShadowBuffer;


class ShadowSlot: public TypedReferenceCount, public GraphicsCallback {

    REGISTER_TYPE("ShadowSlot", TypedReferenceCount)

public:
    virtual ~ShadowSlot();

    virtual void bind(LightBase* light);
    virtual void unbind(LightBase* light);

    void set_index(int index);
    int get_index() const;

    virtual void cull(DisplayRegionCullCallbackData* data) override;
    virtual void draw(DisplayRegionDrawCallbackData* data) override;

protected:
    ShadowSlot(ShadowBuffer* host);

    typedef std::vector<PT(DisplayRegion)> Regions;
    Regions _regions;

    int _index;

    PTA_int _offset;
    PTA_LMatrix4 _projections;

    ShaderState _state;

private:
    static ShadowSlot* make(ShadowBuffer* host);
};


#endif // __SHADOWSLOT_H__