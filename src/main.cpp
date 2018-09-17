#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "mesh_buffer.h"

#include <scene_controller.h>
#include <scene_serializer.h>
#include <external/scene_from_fbx.h>

SceneObject myScene;
SceneObject so;
SceneObject fbx_so;

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    //SerializeScene(&myScene, "test.scn");
    //DeserializeScene("test.scn", so);

    //SceneFromFbx("character.fbx", &fbx_so);
    //SerializeScene(&fbx_so, "fbx_so.scn", true);
    SceneSerializer().Deserialize("fbx_so.scn", *myScene.CreateObject());

    GameState::GetSceneController().SetScene(&myScene);
}