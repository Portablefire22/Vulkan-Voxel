//
// Created by blakey on 27/11/23.
//

#include "Player.h"

#include <map>

#include "../main.h"
#include "../vk_engine.h"

namespace Player {
    std::map<int, bool> keyboard;
    int xMouse, yMouse;
    int lastX, lastY;
    bool firstMouse = true;
    Player::Player(glm::vec3 position, float size) {
        Position = position;
        Size = size;
        camera = Camera(glm::vec3(0.0,0.0,-2.0));
    }

    void Player::processInput() {

        const Uint8* keystate = SDL_GetKeyboardState(NULL);


        if (keystate[SDL_SCANCODE_W]) {
            camera.ProcessKeyboard(FORWARD, entryPoint::engine.deltaTime);
            std::cout << entryPoint::engine.deltaTime << std::endl;
        }
        if (keystate[SDL_SCANCODE_A]) {
            camera.ProcessKeyboard(LEFT, entryPoint::engine.deltaTime);
        }
        if (keystate[SDL_SCANCODE_S]) {
            camera.ProcessKeyboard(BACKWARD, entryPoint::engine.deltaTime);
        }
        if (keystate[SDL_SCANCODE_D]) {
            camera.ProcessKeyboard(RIGHT, entryPoint::engine.deltaTime);
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
            camera.ProcessKeyboard(UP, entryPoint::engine.deltaTime);
        }
        if (keystate[SDL_SCANCODE_LSHIFT]) {
            camera.ProcessKeyboard(DOWN, entryPoint::engine.deltaTime);
        }
    }

    void Player::processMouse(float xOffset, float yOffset) {
        camera.ProcessMouseMovement(xOffset, yOffset, true);
    }



} // Player