#include <panda3d/dconfig.h>
#include "config_shadow.h"
#include "shadowBufferManager.h"
#include "directionalLightShadow.h"
#include "pointLightShadow.h"


ConfigureDef(config_shadow);

ConfigureFn(config_shadow) {
    init_shadow();
}

void init_shadow() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;

    ShadowSlot::init_type();
    DirectionalLightShadow::init_type();
    PointLightShadow::init_type();
}

ConfigVariableString shadow_host_name("shadow-host-name", "",
    PRC_DESC("Name of the windows to use host for the shadow buffers."
            "If it is left empty, the first window will be used."));

ConfigVariableString shadow_main_cam_tag("shadow-main-cam-tag", "",
    PRC_DESC("Only cameras with this tag will be taken into acount for"
            "computing the most influent light sources. If it left empty,"
            "then all cameras will be taken into acount."));

ConfigVariableInt shadow_update_sort("shadow-host-sort", 40,
    PRC_DESC("The sort value of the shadow buffers update task."));

ConfigVariableInt shadow_update_rate("shadow-update-rate", 0.2,
    PRC_DESC("Rate at which update shadows."));