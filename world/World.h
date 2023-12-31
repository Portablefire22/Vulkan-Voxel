//
// Created by blakey on 01/12/23.
//
#pragma once
#ifndef WORLD_H
#define WORLD_H
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include "Chunk.h"
#include "Region.h"
#include "../PerlinNoise.hpp"
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
    std::unordered_map<glm::vec2, std::map<int32_t, chunk::Chunk>> WorldMap;

    std::map<std::pair<int,int>, Region> RegionMap;

    std::vector<chunk::Chunk*> ChunksToRender;
    std::string WorldName;
    int WorldSeed;
    int testI = 0;

    void RenderChunks(VulkanEngine& engine, std::vector<chunk::Chunk*>& chunks);
    void RenderChunk(VulkanEngine& engine, chunk::Chunk* chunk);

    Region* GetRegion(int x, int z);

    std::vector<chunk::Chunk*> GetChunksAroundPlayer(VulkanEngine& engine, Player::Player &player, int horzRenderDistance, int vertRenderDistance);

    chunk::Chunk GetChunk(VulkanEngine& engine, int ChunkX, int ChunkY, int ChunkZ);

    World() {
        WorldName = "Name not Set!";
        WorldSeed = rand() % 32767;
        srand(WorldSeed);
    }
    World(std::string& worldName, const long& worldSeed) {
        WorldName = worldName;
        WorldSeed = worldSeed;
        srand(WorldSeed);
    }

};

} // WorldHandler

#endif //WORLD_H
