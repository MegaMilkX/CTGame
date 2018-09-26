#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#include <component.h>
#include <camera.h>
#include <transform.h>
#include <game_state.h>
#include <light_omni.h>

class FreeCamera : public Component
{
public:
    void OnInit()
    {
        c = Get<Camera>();
        c->Get<Transform>()->Translate(0, 1.6f, 4);

        c->Perspective(1.0f, 16.0f/9.0f, 0.1f, 100.0f);

        GameState::GetInput()->BindAxis("MoveX", std::bind(&FreeCamera::MoveX, this, std::placeholders::_1));        
        GameState::GetInput()->BindAxis("MoveY", std::bind(&FreeCamera::MoveY, this, std::placeholders::_1));
        GameState::GetInput()->BindAxis("MoveCamX", std::bind(&FreeCamera::RotateX, this, std::placeholders::_1));
        GameState::GetInput()->BindAxis("MoveCamY", std::bind(&FreeCamera::RotateY, this, std::placeholders::_1));

        GameState::GetInput()->BindActionPress("Use", std::bind(&FreeCamera::Use, this));
        GameState::GetInput()->BindActionPress("ResetFreeCam", std::bind(&FreeCamera::ResetTransform, this));
    
        GameState::GetInput()->BindActionPress("MouseLookHold", std::bind(&FreeCamera::MouseLookPress, this));
        GameState::GetInput()->BindActionRelease("MouseLookHold", std::bind(&FreeCamera::MouseLookRelease, this));

        LightOmni* light = Get<LightOmni>();
        light->Color(1, 1, 1);
        light->Intensity(1);
    }

    void Use()
    {
        
    }

    void ResetTransform()
    {
        Get<Transform>()->Position(0.0f, 1.6f, 4.0f);
        Get<Transform>()->Rotation(gfxm::quat(0.0f, 0.0f, 0.0f, 1.0f));
    }

    bool mouseLookEnabled = false;
    void MouseLookPress()
    {
        mouseLookEnabled = true;        
    }
    void MouseLookRelease()
    {
        mouseLookEnabled = false;
    }

    void MoveX(float v)
    {
        Get<Transform>()->Translate(v * GameState::DeltaTime() * Get<Transform>()->Right());
    }

    void MoveY(float v)
    {
        Get<Transform>()->Translate(-v * GameState::DeltaTime() * Get<Transform>()->Back());
    }

    void RotateX(float v)
    {
        if(!mouseLookEnabled) return;
        Get<Transform>()->Rotate(-v * 0.005f, gfxm::vec3(0.0f,1.0f,0.0f));
    }

    void RotateY(float v)
    {
        if(!mouseLookEnabled) return;
        Get<Transform>()->Rotate(-v * 0.005f, Get<Transform>()->Right());
    }

    Camera* c;
};

#endif
