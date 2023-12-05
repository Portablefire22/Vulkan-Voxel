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

    std::vector<chunk::Chunk> ChunksToRender;
    char* WorldName;
    char* WorldSeed;

    void CullChunks();
    void TestCreateChunks(VulkanEngine& engine, int width = 16, int height = 16, int depth = 16);
    std::vector<RenderBlock::FACE> CheckBlockFaces(chunk::Chunk* localChunk, glm::vec3* BlockPos);
    void RenderChunks(VulkanEngine& engine, std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction>& chunks);
    std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction> GetChunksAroundPlayer(VulkanEngine& engine, Player::Player &player, int horzRenderDistance, int vertRenderDistance);

    chunk::Chunk GetChunk(VulkanEngine& engine, int ChunkX, int ChunkY, int ChunkZ);
    void GetNoiseHeightMap(chunk::Chunk& localChunk, std::vector<double>* NoiseVec);

    World() {
        WorldName = "Name not Set!";
        WorldSeed = "debug seed";
    }
    World(char* worldName, char* worldSeed) {
        WorldName = worldName;
        WorldSeed = worldSeed;
    }

};

} // WorldHandler

#endif //WORLD_H
