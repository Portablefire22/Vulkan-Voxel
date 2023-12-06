//
// Created by blakey on 27/11/23.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <map>

#include "../Camera.h"
#include "../Entity/Entity.h"

class VulkanEngine;

namespace Player {

class Player{
public:
    Camera camera;
    glm::vec3 Position{};
    glm::vec3 ChunkPosition{};
    glm::vec3 LastChunkPosition{};
    float Size = 1.75f;

    explicit Player(glm::vec3 position = {0,0,0}, float size = 1.75f);
    void processInput(VulkanEngine& engine);
    void updatePosition(VulkanEngine& engine);
    void processMouse(float xOffset, float yOffset);
    void ChunkPositionChanged(VulkanEngine& engine);

    private:
    std::map<int, bool> keyboard;
    int xMouse, yMouse;
    int lastX, lastY;
    bool firstMouse = true;
};

} // Player

#endif //PLAYER_H
