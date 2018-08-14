#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "serialize_scene.h"
#include "deserialize_scene.h"

#include "fbx_read/fbx_read.h"

class ResourceRef
{
public:
    std::string name;
    Resource* resource = 0;
};

SceneObject* CreateOrFindFromModel(SceneObject* scene, Au::Media::FBX::Model* model)
{
    SceneObject* so = scene->FindObject(model->name);
    if(!so)
    {
        so = scene->CreateObject();
        so->Name(model->name);
    }

    return so;
}

bool SceneFromModel(SceneObject* scene, Au::Media::FBX::Model* model)
{


    for(unsigned i = 0; i < model->ChildCount(); ++i)
    {
        auto node = model->GetChild(i);
        auto mdl = Au::Media::FBX::Model(model->settings, model->root, node);

    }

    return true;
}

void SceneFromFbxModel(FbxModel& fbxModel, FbxScene& fbxScene, SceneObject* sceneObject){
    for(unsigned i = 0; i < fbxModel.ChildCount(); ++i)
    {
        SceneObject* child = sceneObject->CreateObject();
        sceneObject->Get<Transform>()->Attach(child->Get<Transform>());

        SceneFromFbxModel(fbxModel.GetChild(i, fbxScene), fbxScene, child);
    }

    sceneObject->Name(fbxModel.GetName());
    LOG("Created object: " << fbxModel.GetName());
    LOG("Type: " << fbxModel.GetType());

    Transform* t = sceneObject->Get<Transform>();
    FbxVector3 pos = fbxModel.GetLclTranslation();
    FbxVector3 rot = fbxModel.GetLclRotation();
    FbxVector3 scl = fbxModel.GetLclScaling();

    //t->Position(*(gfxm::vec3*)&pos);
    //t->Rotation(rot.x, rot.y, rot.z);
    //t->Scale(*(gfxm::vec3*)&scl);

    sceneObject->Get<Transform>()->SetTransform(*(gfxm::mat4*)&fbxModel.GetTransform());
}

void SceneFromFbx(FbxScene& fbxScene, SceneObject* scene){
    for(unsigned i = 0; i < fbxScene.ModelCount(); ++i)
        SceneFromFbxModel(fbxScene.GetModel(i), fbxScene, scene->CreateObject());
}

bool SceneFromFbx(const char* data, size_t size, SceneObject* scene)
{
    FbxScene fbxScene;
    if(!FbxReadMem(fbxScene, data, size))
        return false;
    SceneFromFbx(fbxScene, scene);
    return true;
}

bool SceneFromFbx(const std::string& filename, SceneObject* scene)
{    
    FbxScene fbxScene;
    if(!FbxReadFile(fbxScene, filename))
        return false;
    SceneFromFbx(fbxScene, scene);
    fbxScene._dumpFile(filename);
    return true;
}

SceneObject myScene;
SceneObject so;

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    SerializeScene(&myScene, "test.scn");
    DeserializeScene("test.scn", so);

    SceneFromFbx("character.fbx", myScene.CreateObject());

    GameState::SetScene(&myScene);
}