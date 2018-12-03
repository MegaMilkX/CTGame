#ifndef KINEMATIC_TEST_H
#define KINEMATIC_TEST_H

#include <component.h>
#include <input.h>
#include <transform.h>
#include <common.h>
#include <camera.h>
#include <ghost_collider.h>
#include "debug_draw.h"

#include <scene_controller.h>

class CollisionCallback : public btCollisionWorld::ConvexResultCallback {
public:
    virtual bool needsCollision(btBroadphaseProxy *proxy0) const {
        return true;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace) {
        if(!convexResult.m_hitCollisionObject->isStaticObject()) {
            return 0.0f;
        }

        if(convexResult.m_localShapeInfo) {
            std::cout << "Tri index: " << convexResult.m_localShapeInfo->m_triangleIndex << std::endl;
            
        }

        const ddVec3 f  = { 
            convexResult.m_hitPointLocal.getX(), 
            convexResult.m_hitPointLocal.getY(), 
            convexResult.m_hitPointLocal.getZ() 
        };
        const ddVec3 t = { 
            convexResult.m_hitPointLocal.getX() + convexResult.m_hitNormalLocal.getX(), 
            convexResult.m_hitPointLocal.getY() + convexResult.m_hitNormalLocal.getY(), 
            convexResult.m_hitPointLocal.getZ() + convexResult.m_hitNormalLocal.getZ() 
        };
        const ddVec3 col = { 1.0f, 0.0f, 0.5f };
        if(normalInWorldSpace)
        {
            dd::line(f, t, col, 0, false);
        }
        dd::point(f, col);
        hitNormal = gfxm::normalize(hitNormal + gfxm::vec3(
            convexResult.m_hitNormalLocal.getX(),
            convexResult.m_hitNormalLocal.getY(),
            convexResult.m_hitNormalLocal.getZ()
        ));
        return convexResult.m_hitFraction;
    }

    gfxm::vec3 hitNormal;
};

class KinematicTest : public Component {
    RTTR_ENABLE(Component)
public:
    virtual void OnInit() {
        t = Get<Transform>();
        gInput.BindAxis(
            "MoveHori",
            [this](float v){
                if(Common.camera) {
                    btCapsuleShape capsule(0.3f, 1.4f);

                    gfxm::vec3 right = ((Camera*)Common.camera)->Get<Transform>()->Right();
                    right.y = 0.0f;
                    right = gfxm::normalize(right);
                    gfxm::vec3 delta = right * v;
                    float mod = Common.frameDelta * 2.5f;
                    
                    if(Object()->GetController()->GetPhysics().GetBtWorld()) {
                        //btVector3 vfrom = *(btVector3*)&t->WorldPosition();
                        //btVector3 vto = *(btVector3*)&(t->WorldPosition() + delta);

                        gfxm::vec3 pos = t->WorldPosition();
                        gfxm::quat rot = t->WorldRotation();
                        
                        btTransform from;
                        from.setRotation(btQuaternion(rot.x,rot.y,rot.z,rot.w));
                        from.setOrigin(btVector3(pos.x,pos.y,pos.z));
                        
                        btTransform to;
                        to.setRotation(btQuaternion(rot.x,rot.y,rot.z,rot.w));
                        pos = pos + delta;
                        to.setOrigin(btVector3(pos.x,pos.y,pos.z));

                        CollisionCallback callback;
                        Object()->GetController()->GetPhysics().GetBtWorld()->convexSweepTest(&capsule, from, to, callback);
                        //if(callback.hasHit()) {
                            gfxm::vec3 normal = gfxm::normalize(callback.hitNormal);
                            delta = gfxm::normalize(normal - delta);
                            {
                                const ddVec3 fr  = { 
                                    pos.x, 
                                    pos.y, 
                                    pos.z 
                                };
                                const ddVec3 to = { 
                                    pos.x + normal.x, 
                                    pos.y + normal.y, 
                                    pos.z + normal.z 
                                };
                                const ddVec3 col = { 0.0f, 0.0f, 1.0f };
                                dd::line(fr, to, col, 0, false);
                            }
                        //}
                    }

                    {
                        gfxm::vec3 pos = t->WorldPosition();

                        const ddVec3 fr  = { 
                            pos.x, 
                            pos.y, 
                            pos.z 
                        };
                        const ddVec3 to = { 
                            pos.x + delta.x, 
                            pos.y + delta.y, 
                            pos.z + delta.z 
                        };
                        const ddVec3 col = { 0.0f, 1.0f, 0.5f };
                        dd::line(fr, to, col, 0, false);
                    }
                    t->Translate(delta * mod);
                }
            }
        );
        gInput.BindAxis(
            "MoveVert",
            [this](float v) {
                gfxm::vec3 forward = ((Camera*)Common.camera)->Get<Transform>()->Forward();
                forward.y = 0.0f;
                forward = gfxm::normalize(forward);
                t->Translate(forward * v * Common.frameDelta * 2.5f);
            }
        );
    }
private:
    Transform* t;
};
STATIC_RUN(KinematicTest)
{
    rttr::registration::class_<KinematicTest>("KinematicTest")
        .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

#endif
