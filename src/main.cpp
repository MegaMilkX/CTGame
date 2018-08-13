#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "serialize_scene.h"
#include "deserialize_scene.h"

#include "fbx/fbx_reader.h"
#include "fbx/fbx_model_tree.h"

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

bool SceneFromFbx(char* data, size_t size, SceneObject* scene)
{
    FbxReader reader;
    FbxScene fbxScene;
    reader.ReadMem(data, size, fbxScene);

    FbxModelTree modelTree(reader.GetRootNode());

    return true;
}

bool SceneFromFbx(const std::string& filename, SceneObject* scene)
{    
    std::ifstream f(filename, std::ios::binary | std::ios::ate);
    if(!f.is_open())
    {
        std::cout << "Failed to open " << filename << std::endl;
        return false;
    }
    std::streamsize size = f.tellg();
    f.seekg(0, std::ios::beg);
    std::vector<char> buffer((unsigned int)size);
    if(!f.read(buffer.data(), (unsigned int)size))
    {
        f.close();
        return false;
    }

    SceneFromFbx(buffer.data(), buffer.size(), scene);

    f.close();
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