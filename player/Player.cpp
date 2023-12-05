//
// Created by blakey on 27/11/23.
//

#include "Player.h"

#include <map>
#include <SDL_keyboard.h>

#include "../main.h"
#include "../vk_engine.h"

namespace Player {

    Player::Player(glm::vec3 position, float size) {
        Position = position;
        Size = size;
        camera = Camera(glm::vec3(0.0,0.0,0.0));
    }

    void Player::processInput() { // Called every frame

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
        updatePosition();
    }

    void Player::updatePosition() {
        Position = camera.Position;
        Position.y -= 1.75f;
        ChunkPosition = {floor(Position.x / CHUNK_SIZE), floor(Position.y / CHUNK_SIZE),floor(Position.z / CHUNK_SIZE)};
    }

    void Player::processMouse(float xOffset, float yOffset) {
        camera.ProcessMouseMovement(xOffset, yOffset, true);
    }



} // Player