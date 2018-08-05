#include <aurora2/aurora2.h>


#include "world_controller.h"

SceneObject myScene;

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    GameState::SetScene(&myScene);

    //GameState::Push<StateTest>();
}