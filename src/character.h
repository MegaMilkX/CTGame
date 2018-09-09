#ifndef CHARACTER_H
#define CHARACTER_H

#include <scene_object.h>
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

        //cam = GetObject()->Root()->CreateObject()->Get<CharacterCamera>();
        //cam->SetTarget(m->GetObject()->FindObject("Root")->Get<Transform>());


    }

    void OnUpdate()
    {
    }

    void OnCleanup()
    {

    }

    Model* m;

    CharacterCamera* cam = 0;
};

#endif
