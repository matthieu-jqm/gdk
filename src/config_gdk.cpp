#include <panda3d/dconfig.h>

#include "scene.h"
#include "serializable.h"
#include "config_render_pipeline.h"
#include "config_light.h"
#include "config_shadow.h"
#include "config_anim.h"
#include "config_instance.h"
#include "config_gdk.h"

ConfigureDef(config_gdk);

ConfigureFn(config_gdk) {
    init_light();
}

NotifyCategoryDef(gdk, "");

void init_base() {

    Scene::init_type();
    Serializable::init_type();
}


void init_gdk() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;

    init_base();
    init_light();
    init_shadow();
    init_anim();
    init_instance();
    init_render_pipeline();
}
