#ifndef STATE_LOADING_H
#define STATE_LOADING_H

#include <scene.h>

#include <game_state.h>
#include <model.h>
#include <texture2d.h>
#include <camera.h>
#include <transform.h>
#include <light_omni.h>
#include <functional>
#include <overlay/overlay_root.h>
//#include <util/handle.h>

#include <component.h>

#include <rttr/registration>

#include <util/binary_codec.h>

#define PROPERTY(COMPONENT, PROP) \
int COMPONENT ## PROP ## _dummy = 10; \

class Component2 : public SceneObject::Component
{
public:
    int prop;
    std::string GetName() const { return "hello"; }
    void SetName(std::string name) { }
    void Foo() 
    {
        std::cout << "Foo" << std::endl;
    }
};
COMPONENT(Component2)
PROPERTY(Component2, prop)

#include <util/static_run.h>

STATIC_RUN(Hello){
    std::cout << "Hello, World!" << std::endl;
    Meta::Init<Component2>("Component2");

    rttr::registration::class_<Component2>("Component2")
        .constructor<>()(rttr::policy::ctor::as_raw_ptr)
        .property("prop", &Component2::prop)
        .property("name", &Component2::GetName, &Component2::SetName);
    
    return 0;
}

class StateLoading : public GameState
{
public:
    virtual void OnInit() 
    {
        binary::binary_codec codec;
        codec.add_chunk(1, "MyNumber");
        codec.add_chunk('q', "Character");
        codec.save("test.bin");

        rttr::type t = rttr::type::get_by_name("Component2");
        rttr::variant v = t.create();
        Component2 c = v.get_value<Component2>();
        std::cout << t.get_name() << std::endl;
        
        t.get_property("prop").set_value(c, 20);
        std::cout << c.prop << std::endl;
        std::cout << v.get_type().get_name() << std::endl;
        if(v.get_type() == rttr::type::get<Component2*>())
        {
            std::cout << "yes" << std::endl;
        }
        

        renderer = scene.GetComponent<Renderer>();
        std::cout << Component2prop_dummy << std::endl;
        Blueprint bp;
        bp.Name("MyLight");
        bp.AddComponent("LightOmni");

        scene.Instantiate(bp);
        SceneObject* o = scene.FindObject("MyLight");
        if(o)
        {
            std::cout << "Created object" << std::endl;
            LightOmni* l = o->FindComponent<LightOmni>();
            if(l)
            {
                std::cout << "Created component" << std::endl;
                l->Color(1.0f, 0.0f, 0.0f);
                l->Get<Transform>()->Translate(0.0f, 0.3f, 0.4f);
            }
        }

        cam = scene.CreateObject()->Get<Camera>();
        cam->Perspective(1.4f, 16.0f/9.0f, 0.5f, 100.0f);
        cam->Get<Transform>()->Translate(0.0f, 1.5f, 1.0f);

        mesh = scene.CreateObject()->Get<Model>();
        mesh->mesh.set("stylized");
        mesh->material.set("material");

        LightOmni* omni = scene.CreateObject()->Get<LightOmni>();
        omni->Color(1.0f, 1.0f, 1.0f);
        omni->Get<Transform>()->Translate(0.0f, 1.5f, 1.0f);

        Quad* quad = scene.CreateObject()->GetComponent<Quad>();
        quad->SetImage("V8fBNZhT");
        quad->SetSize(400, 250);
    }
    virtual void OnCleanup() 
    {
    }
    virtual void OnUpdate() 
    {
        scene.Update();
        mesh->Get<Transform>()->Rotate(0.5f * DeltaTime(), gfxm::vec3(0.0f, 1.0f, 0.0f));
    }
    virtual void OnRender()
    {
        renderer->Render();
    }
private:
    Renderer* renderer;
    Camera* cam;
    Model* mesh;
    Scene scene;
};

#endif
