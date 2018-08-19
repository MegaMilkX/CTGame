#ifndef CHARACTER_H
#define CHARACTER_H

#include <scene_object.h>
#include <skeleton.h>
#include <animator.h>
#include <model.h>

#include "character_camera.h"

class Character : public Updatable
{
public:
    void OnStart()
    {
        m = Get<Model>();
        m->mesh.Set("General.Player.Mesh.geo");
        m->material.Set("General.Material.material2");

        m->Get<Skeleton>()->SetData("General.Player.Skeleton");

        anim = m->Get<Animator>();
        anim->Set("General.Player.Animation");
        anim->Play("Run");

        layerCursor = anim->GetAnimCursor("LayerMotion01");

        //cam = GetObject()->Root()->CreateObject()->Get<CharacterCamera>();
        //cam->SetTarget(m->GetObject()->FindObject("Root")->Get<Transform>());


    }

    void OnUpdate()
    {
        layerCursor += GameState::DeltaTime();
        anim->ApplyAdd(layerCursor, 1.0f);
    }

    void OnCleanup()
    {

    }

    Model* m;

    CharacterCamera* cam = 0;

    Animator* anim;
    AnimTrack::Cursor layerCursor;
};

#endif
