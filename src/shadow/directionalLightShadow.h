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

#ifndef __DIRECTIONALLIGHTSHADOW_H__
#define __DIRECTIONALLIGHTSHADOW_H__

#include "shadowBuffer.h"
#include "shadowSlot.h"

extern ConfigVariableInt num_shadow_cascade;
extern  ConfigVariableDouble csm_log_factor;



class DirectionalLightShadow: public ShadowSlot {

    REGISTER_TYPE("DirectionalLightShadow", ShadowSlot)

public:
    static ShadowBufferInfo get_info();
    
    virtual ~DirectionalLightShadow() = default;

protected:
    DirectionalLightShadow(ShadowBuffer* buffer);

private:
    static ShadowSlot* make(ShadowBuffer* buffer);

    static void compute_csm(SceneSetup* setup, LightLensNode* node);
    static float compute_split_depth(uint8_t n);

    static uint8_t _num_splits;
    static float _log_factor;
};

#endif // __DIRECTIONALLIGHTSHADOW_H__