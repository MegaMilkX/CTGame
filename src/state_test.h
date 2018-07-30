#ifndef STATE_TEST_H
#define STATE_TEST_H

#include <game_state.h>
#include <scene_object.h>
#include <model.h>
#include <camera.h>
#include <transform.h>
#include <animator.h>
#include <skeleton.h>
#include <light_omni.h>

class StateTest : public GameState
{
public:
    void OnInit()
    {
        Model* m = scene.CreateObject()->Get<Model>();
        m->mesh.set("General.Player.Mesh");
        m->material.set("General.Material.material2");

        Camera* c = scene.CreateObject()->Get<Camera>();
        c->Get<Transform>()->Translate(0, 1.5, 1);

        m->Get<Skeleton>()->SetData("General.Player.Skeleton");

        anim = m->Get<Animator>();
        anim->Set("General.Player.Animation");
        anim->Play("Idle");
        layerMotion = anim->GetAnimCursor("LayerMotion01");

        LightOmni* light = scene.Get<LightOmni>();
        light->Color(1, 1, 1);
        light->Intensity(1);
        light->Get<Transform>()->Translate(0, 1.5, 1);
    }
    AnimTrack::Cursor layerMotion;
    Animator* anim;
    void OnCleanup()
    {

    }

    void OnUpdate()
    {
        layerMotion += GameState::DeltaTime();
        anim->Tick(GameState::DeltaTime());
        anim->ApplyAdd(layerMotion, 1.0f);
        anim->Finalize();
    }

    void OnRender()
    {
        scene.Get<Renderer>()->Render();
    }
private:
    SceneObject scene;
};

#endif
