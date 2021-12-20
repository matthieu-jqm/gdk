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

#ifndef __POINTLIGHTSHADOW_H__
#define __POINTLIGHTSHADOW_H__

#include "multiViewShadowSlot.h"
#include "shadowBuffer.h"


class PointLightShadow: public MultiViewShadowSlot {

    REGISTER_TYPE("PointLightShadow", MultiViewShadowSlot)

public:
    static ShadowBufferInfo get_info();

    virtual ~PointLightShadow() = default;

protected:
    PointLightShadow(ShadowBuffer* buffer);

    virtual void cull(DisplayRegionCullCallbackData* data) override;

private:
    static void post_init();
    static ShadowSlot* make(ShadowBuffer* buffer);

    typedef std::array<LMatrix4, 6> Faces;
    static Faces _faces;
};


#endif // __POINTLIGHTSHADOW_H__