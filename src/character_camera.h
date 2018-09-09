#ifndef CHARACTER_CAMERA_H
#define CHARACTER_CAMERA_H

#include <scene_object.h>
#include <camera.h>
#include <transform.h>
#include <game_state.h>

class CharacterCamera : public Updatable
{
public:
    void OnStart()
    {
        c = CreateObject()->Get<Camera>();
        c->Get<Transform>()->Translate(0, 1.6, 4);

        c->Perspective(1.0f, 16.0f/9.0f, 0.1f, 100.0f);
    }

    void OnUpdate()
    {
        if(!target) return;

        c->Get<Transform>()->Position(target->WorldPosition() + gfxm::vec3(0.0f,1.6f,4.0f));
    }

    void SetTarget(Transform* tgt)
    {
        target = tgt;
    }

private:
    Camera* c;
    Transform* target = 0;
};

#endif
