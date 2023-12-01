//
// Created by blakey on 01/12/23.
//
#pragma once
#ifndef WORLD_H
#define WORLD_H
#include <map>
#include <set>
#include <unordered_map>
#include <glm/vec2.hpp>

#include "chunk.h"
#include "../player/Player.h"
#include "../RenderUtils/RenderBlock.h"

class VulkanEngine;

namespace WorldHandler {

class World {
    // So this issue is not in minecraft, how do I have infinite generation going horizontally and vertically ?
    // Suppose I use a map to hold the overall world & vertical chunks
public:
    // TODO FIX
    // https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
    std::unordered_map<glm::vec2, std::map<int32_t, chunk::chunk>> WorldMap;

    std::vector<chunk::chunk> ChunksToRender;
    char* WorldName;

    void CullChunks();
    void TestCreateChunks(VulkanEngine& engine, int width = 16, int height = 16, int depth = 16);
    std::vector<RenderBlock::FACE> CheckBlockFaces(chunk::chunk localChunk, glm::vec3 BlockPos);
    void RenderChunks(VulkanEngine& engine, std::set<chunk::chunk> chunks);
    std::set<chunk::chunk> GetChunksAroundPlayer(Player::Player &player, int horzRenderDistance, int vertRenderDistance);

    chunk::chunk GetChunk(int ChunkX, int ChunkY, int ChunkZ);

    World() {
        WorldName = "Name not Set!";
    }
    World(char* worldName) {
        WorldName = worldName;
    }

};

} // WorldHandler

#endif //WORLD_H
