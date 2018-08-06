#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include <updatable.h>
#include <debug_draw.h>
#include <transform.h>

class DebugDrawer : public Updatable
{
public:
    virtual void OnUpdate()
    {
        auto transforms = Object()->FindAllOf<Transform>();
        for(auto t : transforms)
        {
            gfxm::mat4 m = t->GetTransform();
            gfxm::vec4 pos = m[3];
            gfxm::mat3 o = gfxm::to_orient_mat3(m);
            m = o;
            m[3] = pos;
            dd::axisTriad((float*)&m, 0.01f, 0.1f, 0, false);
        }
    }
};

#endif
