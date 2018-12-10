#ifndef KINEMATIC_TEST_2_H
#define KINEMATIC_TEST_2_H

#include <component.h>
#include <updatable.h>
#include <input.h>
#include <transform.h>
#include <common.h>
#include <camera.h>
#include <ghost_collider.h>
#include "debug_draw.h"

#include <scene_controller.h>

class KinematicTest2 : public Updatable {
    RTTR_ENABLE(Updatable)
public:
    virtual void OnInit() {
        prev_pos = Get<Transform>()->Position();
    }
    virtual void OnUpdate() {
        gfxm::vec3 curr_pos = Get<Transform>()->Position();

        //Object()->GetController()->GetPhysics().GetBtWorld()->convexSweepTest();
        // TODO:
        btCollisionObject* co;
        //co->

        prev_pos = curr_pos;
    }
private:
    gfxm::vec3 prev_pos;
};
STATIC_RUN(KinematicTest2)
{
    rttr::registration::class_<KinematicTest2>("KinematicTest2")
        .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

#endif
