#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#include <scene_object.h>
#include <camera.h>
#include <transform.h>
#include <game_state.h>

class FreeCamera : public SceneObject::Component
{
public:
    void OnInit()
    {
        c = CreateObject()->Get<Camera>();
        c->Get<Transform>()->Translate(0, 1.6, 4);
        GetObject()->Root()->Get<Renderer>()->CurrentCamera(c);

        c->Perspective(1.0f, 16.0f/9.0f, 0.1f, 100.0f);

        GameState::GetInput()->BindAxis("MoveX", std::bind(&FreeCamera::MoveX, this, std::placeholders::_1));        
        GameState::GetInput()->BindAxis("MoveY", std::bind(&FreeCamera::MoveY, this, std::placeholders::_1));
        GameState::GetInput()->BindAxis("MoveCamX", std::bind(&FreeCamera::RotateX, this, std::placeholders::_1));
        GameState::GetInput()->BindAxis("MoveCamY", std::bind(&FreeCamera::RotateY, this, std::placeholders::_1));

        GameState::GetInput()->BindActionPress("Use", std::bind(&FreeCamera::Use, this));
        GameState::GetInput()->BindActionPress("ResetFreeCam", std::bind(&FreeCamera::ResetTransform, this));
    }

    void Use()
    {
        
    }

    void ResetTransform()
    {
        c->Get<Transform>()->Position(0.0f, 1.6f, 4.0f);
        c->Get<Transform>()->Rotation(gfxm::quat(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void MoveX(float v)
    {
        c->Get<Transform>()->Translate(v * GameState::DeltaTime() * c->Get<Transform>()->Right());
    }

    void MoveY(float v)
    {
        c->Get<Transform>()->Translate(-v * GameState::DeltaTime() * c->Get<Transform>()->Back());
    }

    void RotateX(float v)
    {
        c->Get<Transform>()->Rotate(-v * 0.005, gfxm::vec3(0.0f,1.0f,0.0f));
    }

    void RotateY(float v)
    {
        c->Get<Transform>()->Rotate(-v * 0.005, c->Get<Transform>()->Right());
    }

    Camera* c;
};

#endif
