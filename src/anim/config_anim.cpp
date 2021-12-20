#include <panda3d/dconfig.h>
#include "config_anim.h"
#include "animSequence.h"
#include "animSequenceControl.h"


ConfigureDef(config_anim);

ConfigureFn(config_anim) {
    init_anim();
}


void init_anim() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;

    AnimSequence::init_type();
    AnimSequenceControl::init_type();
    SequenceInterval::init_type();

    AnimSequence::register_with_read_factory();
}
