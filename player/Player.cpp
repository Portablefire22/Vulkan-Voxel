//
// Created by blakey on 27/11/23.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "Player.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <iostream>
#include <map>
#include <SDL_keyboard.h>
#include <glm/gtx/string_cast.hpp>

#include "../main.h"
#include "../vulkan/vk_engine.h"

namespace Player {

Player::Player(glm::vec3 position, float size)
{
    this->setPosition(position);
    this->setSize(1.75f, 0.5f);
    camera = Camera(glm::vec3(0.0, 0.0, 0.0));
}

void
Player::processInput(VulkanEngine& engine)
{ // Called every frame

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_W]) {
        camera.ProcessKeyboard(FORWARD, entryPoint::engine.deltaTime);
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
    updatePosition(engine);
}

} // Player
