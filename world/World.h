//
// Created by blakey on 01/12/23.
//
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#ifndef WORLD_H
#define WORLD_H
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <map>
#include <string>
#include <unordered_map>

#include "../player/Player.h"
#include "Chunk.h"
#include "Region.h"

#include "../threadpool/ThreadPool.hpp"

class VulkanEngine;
struct RenderObject;

namespace WorldHandler {

class World
{
    // So this issue is not in minecraft, how do I have infinite generation
    // going horizontally and vertically ? Suppose I use a map to hold the
    // overall world & vertical chunks
  public:
    // TODO FIX
    // https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
    std::unordered_map<glm::vec2, std::map<int32_t, chunk::Chunk>> WorldMap;

    std::map<std::pair<int, int>, Region> RegionMap;

    std::vector<chunk::Chunk*> ChunksToRender;
    std::string WorldName;
    long WorldSeed;
    int testI = 0;

    RenderObject SetToRender(VulkanEngine &engine, chunk::Chunk localChunk);

    void RenderChunks(VulkanEngine& engine, std::vector<chunk::Chunk*>& chunks);
    void RenderChunk(VulkanEngine& engine, chunk::Chunk* chunk);

    Region* GetRegion(int x, int z);

    std::queue<chunk::Chunk*> GetChunksAroundPlayer(VulkanEngine& engine,
                                                     Player::Player& player,
                                                     int horzRenderDistance,
                                                     int vertRenderDistance
                                                    );

    chunk::Chunk GetChunk(VulkanEngine& engine,
                          int ChunkX,
                          int ChunkY,
                          int ChunkZ);

    World()
    {
        WorldName = "Name not Set!";
        WorldSeed = rand() % 32767;
        srand(WorldSeed);
    }
    World(std::string& worldName, const long& worldSeed)
    {
        WorldName = worldName;
        WorldSeed = worldSeed;
        srand(WorldSeed);
    }
};

} // WorldHandler

#endif // WORLD_H
