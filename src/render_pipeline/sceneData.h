

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

#ifndef __SCENEDATA_H__
#define __SCENEDATA_H__

#include <panda3d/sceneSetup.h>
#include "utils.h"


class SceneData: public SceneSetup {

    REGISTER_TYPE("SceneData", SceneSetup)

public:
    SceneData();
    ~SceneData() = default;

    void update_data(SceneSetup* setup);

    PTA_LVecBase2 _near_far;
    PTA_LVecBase2 _view_scale;
    PTA_LVecBase2i _resolution;
};




#endif // __SCENEDATA_H__