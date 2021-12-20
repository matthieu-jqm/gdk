#include "lightCallbackData.h"


DEFINE_TYPEHANDLE(LightCallbackData)



LightCallbackData::LightCallbackData(LightBase* light, Action action):
    _light(light), _action(action)
{
    
}

LightCallbackData::LightCallbackData(LightBase* light, Action action, 
    CullTraverser* trav, CullTraverserData* cull_data): 
    _light(light), _action(action), _trav(trav), _cull_data(cull_data)
{
    
}


LightBase* LightCallbackData::get_light() const {
    
    return _light;
}


LightCallbackData::Action LightCallbackData::get_action() const {

    return _action;    
}


CullTraverserData* LightCallbackData::get_cull_data() const {

    return _cull_data;    
}


CullTraverser* LightCallbackData::get_traverser() const {
    
    return _trav;
}
