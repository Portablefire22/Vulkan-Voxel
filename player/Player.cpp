//
// Created by blakey on 27/11/23.
//

#include "Player.h"

#include <iostream>
#include <map>
#include <SDL_keyboard.h>
#include <glm/gtx/string_cast.hpp>

#include "../main.h"
#include "../vk_engine.h"



namespace Player {

    Player::Player(glm::vec3 position, float size) {
        Position = position;
        Size = size;
        camera = Camera(glm::vec3(0.0,0.0,0.0));
    }

    void Player::processInput(VulkanEngine& engine) { // Called every frame

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

    void Player::updatePosition(VulkanEngine& engine) {
        Position = camera.Position;
        Position.y -= 1.75f;
        ChunkPosition = {floor(Position.x / CHUNK_SIZE), floor(Position.y / CHUNK_SIZE),floor(Position.z / CHUNK_SIZE)};
        if (ChunkPosition != LastChunkPosition) {
            ChunkPositionChanged(engine);
            LastChunkPosition = ChunkPosition;
        }
    }


    void Player::ChunkPositionChanged(VulkanEngine& engine) {
        engine.chunksToRender = engine.currentWorld.GetChunksAroundPlayer(engine, *this, HorzRenderDist, VertRenderDist);
        engine.currentWorld.RenderChunks(engine, engine.chunksToRender);
        for (auto iter = engine._renderables.begin(); iter != engine._renderables.end();) {
            if (abs(iter->position.y - this->ChunkPosition.y) > VertRenderDist || abs(iter->position.x - this->ChunkPosition.x) > HorzRenderDist || abs(iter->position.z - this->ChunkPosition.z) > HorzRenderDist) {
                engine._renderables.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    void Player::processMouse(float xOffset, float yOffset) {
        camera.ProcessMouseMovement(xOffset, yOffset, true);
    }

} // Player