

#include <aurora2/aurora2.h>
#include "state_test.h"

static SceneObject scene;

void Aurora2Init()
{
    GameState::Push<StateTest>();
}