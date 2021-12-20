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

#include "lightData.h"
#include "pointLightNode.h"
#include "spotLightNode.h"
#include "tubeLightNode.h"
#include "areaLightNode.h"
#include "directionalLightNode.h"


using namespace std;


string get_light_name(LightType type) {
    
    switch (type) {
        case LT_point: return "point";
        case LT_spot: return "spot";
        case LT_area: return "area";
        case LT_tube: return "tube";
        case LT_directional: return "directional";
    }
    return "unknown";
}


BIND_LIGHT_TYPE(LT_point, PointLightNode)
BIND_LIGHT_TYPE(LT_spot, SpotLightNode)
BIND_LIGHT_TYPE(LT_tube, TubeLightNode)
BIND_LIGHT_TYPE(LT_directional, DirectionalLightNode)