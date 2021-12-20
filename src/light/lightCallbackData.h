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


#ifndef __LIGHTCALLBACKDATA_H__
#define __LIGHTCALLBACKDATA_H__


#include <panda3d/callbackData.h>

#include "utils.h"

class LightBase;
class CullTraverser;
class CullTraverserData;

class LightCallbackData : public CallbackData {

    REGISTER_TYPE("LightCallbackData", CallbackData)

public:
    enum Action {
        L_create = 0,
        L_release,
        L_add_for_draw
    };

    LightCallbackData(LightBase* light, Action action);
    
    LightCallbackData(LightBase* light, Action action, 
                    CullTraverser* trav, CullTraverserData* cull_data);
       
    LightBase* get_light() const;
    Action get_action() const;
    CullTraverser* get_traverser() const;
    CullTraverserData* get_cull_data() const;

private:
    Action _action;
    LightBase* _light;
    CullTraverser* _trav;
    CullTraverserData* _cull_data;
};
#endif // __LIGHTCALLBACKDATA_H__