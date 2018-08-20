#ifndef WORLD_CONTROLLER_H
#define WORLD_CONTROLLER_H

#include <updatable.h>
#include <transform.h>
#include <light_omni.h>
#include <sound_emitter.h>

#include "character.h"
#include "free_camera.h"

class WorldController : public Updatable
{
public:
    void OnStart()
    {
        LOG("Hello");

        freeCamera = CreateObject()->Get<FreeCamera>();
        LightOmni* cl = freeCamera->Get<LightOmni>();    
        cl->Color(1, 1, 1);
        cl->Intensity(1);

        LightOmni* light = CreateObject()->Get<LightOmni>();
        light->Color(1, 1, 1);
        light->Intensity(1);
        light->Get<Transform>()->Translate(0, 0.3, 1);

        //Model* env = CreateObject()->Get<Model>();
        //env->mesh.set("General.Model.Room");
        //env->material.set("General.Material.material1");

        LightDirect* ld = Get<LightDirect>();
        ld->Color(1,1,1);
        ld->Direction(0, -1, 0);

        //chara = CreateObject()->Get<Character>();

        Model* m = CreateObject()->Get<Model>();
        m->SetMesh("General.Model.miku2.geo");
        m->SetMaterial("General.Material.mat");
        //m->Get<Transform>()->Scale(30.0f);
    }

    void OnUpdate()
    {
        
    }

    void OnCleanup()
    {

    }

    FreeCamera* freeCamera;
    Character* chara;
};

#endif
