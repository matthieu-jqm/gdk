#include <panda3d/dconfig.h>
#include "config_light.h"
#include "pointLightNode.h"
#include "spotLightNode.h"
#include "areaLightNode.h"
#include "tubeLightNode.h"
#include "directionalLightNode.h"
#include "shadowBuffer.h"


ConfigureDef(config_light);

ConfigureFn(config_light) {
    init_light();
}


void init_light() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;

    LightBase::init_type();
    PointLightNode::init_type();
    SpotLightNode::init_type();
    AreaLightNode::init_type();
    TubeLightNode::init_type();
    DirectionalLightNode::init_type();
}
