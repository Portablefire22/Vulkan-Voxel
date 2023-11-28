//
// Created by blakey on 27/11/23.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_events.h>

#include "../Camera.h"

namespace Player {

class Player {
public:
    Camera camera;
    glm::vec3 Position{};
    float Size = 1.75f;

    explicit Player(glm::vec3 position = {0,0,0}, float size = 1.75f);
    void processInput();
    void processMouse(float xOffset, float yOffset);
};

} // Player

#endif //PLAYER_H
