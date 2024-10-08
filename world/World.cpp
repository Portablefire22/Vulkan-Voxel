//
// Created by blakey on 01/12/23.
//
#include "Chunk.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <syncstream>
#include <thread>
#include <utility>
#define GLM_ENABLE_EXPERIMENTAL
#include "World.h"

#include "../vulkan/vk_engine.h"
#include <cstdlib>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_query.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL_timer.h>

namespace WorldHandler {

RenderObject
World::SetToRender(chunk::Chunk localChunk)
{
    RenderObject chunkObject;
    // std::string name = glm::to_string(localChunk->ChunkPosition);

    // bool chunkExists = false;
    // for (auto t : engine._renderables) {
    //     if (t.name == name) {
    //         return t;
    //     }
    // }

    // Check if the chunk exists or not

    // Check if the chunk mesh already exists, if not then create it
    // auto t = localChunk->GenerateChunkMesh();
    // chunkObject.mesh = &t;
    // chunkObject.name = name;

    // chunkObject.material = engine.getMaterial("grass");

    // glm::mat4 translation = glm::translate(
    //   glm::mat4{ 1.0 }, localChunk->ChunkPosition * (float)CHUNK_SIZE);
    // glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(1, 1, 1));
    // chunkObject.transformMatrix = translation * scale;
    // chunkObject.position = localChunk->ChunkPosition;
    // std::osyncstream(std::cout) << "Pushing Mesh: [" << localChunk->ChunkPosition.x << ","
    //           << localChunk->ChunkPosition.y << ","
    //           << localChunk->ChunkPosition.z << "]" << "Thread ID: " << std::this_thread::get_id() << '\n';
    return chunkObject;
}

void
World::RenderChunks(VulkanEngine& engine, std::vector<chunk::Chunk*>& chunks)
{
    for (chunk::Chunk* localChunk : chunks) {
        std::string name = glm::to_string(localChunk->ChunkPosition);
        bool chunkExists = false;
        for (auto t : engine._renderables) {
            if (t.name == name) {
                chunkExists = true;
            }
        }
        if (chunkExists) {
            continue;
        }

        RenderObject chunkObject;

        // Check if the chunk mesh already exists, if not then create it
        if (!engine._meshes.contains(name)) {
            localChunk->GenerateChunkMesh();
        }
        chunkObject.mesh = engine.getMesh(name);
        if (chunkObject.mesh == nullptr) {
            continue;
        }
        chunkObject.name = name;

        chunkObject.material = engine.getMaterial("grass");

        glm::mat4 translation = glm::translate(
          glm::mat4{ 1.0 }, localChunk->ChunkPosition * (float)CHUNK_SIZE);
        glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(1, 1, 1));
        chunkObject.transformMatrix = translation * scale;
        chunkObject.position = localChunk->ChunkPosition;
        engine._renderables.push_back(chunkObject);
    }
}

void
World::RenderChunk(VulkanEngine& engine, chunk::Chunk* localChunk)
{
    RenderObject chunkObject;
    std::string name = glm::to_string(localChunk->ChunkPosition);

    // bool chunkExists = false;
    // for (auto t : engine._renderables) {
    //     if (t.name == name) {
    //         chunkExists = true;
    //     }
    // }
    // if (chunkExists) {
    //     return;
    // }

    // // Check if the chunk mesh already exists, if not then create it
    // if (!engine._meshes.contains(name)) {
    //     localChunk->GenerateChunkMesh();
    // }
    chunkObject.mesh = engine.getMesh(name);
    if (chunkObject.mesh == nullptr) {
        return;
    }
    chunkObject.name = name;

    chunkObject.material = engine.getMaterial("grass");

    glm::mat4 translation = glm::translate(
      glm::mat4{ 1.0 }, localChunk->ChunkPosition * (float)CHUNK_SIZE);
    glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(1, 1, 1));
    chunkObject.transformMatrix = translation * scale;
    chunkObject.position = localChunk->ChunkPosition;
    engine._renderables.push_back(chunkObject);
}

// https://github.com/jdah/minecraft-again/blob/master/src/level/area.cpp <=
// Probably a good starting point

std::queue<chunk::Chunk*>
World::GetChunksAroundPlayer(VulkanEngine& engine,
                             Player::Player& player,
                             int horzRenderDistance,
                             int vertRenderDistance)
{
    std::queue<chunk::Chunk*> chunksToRender;
    for (int z = player.ChunkPosition.z - horzRenderDistance;
         z <= player.ChunkPosition.z + horzRenderDistance;
         z++) {
        for (int x = player.ChunkPosition.x - horzRenderDistance;
             x <= player.ChunkPosition.x + horzRenderDistance;
             x++) {
            auto tempRegion = GetRegion(x, z);
            for (int y = player.ChunkPosition.y - vertRenderDistance;
                 y <= player.ChunkPosition.y + vertRenderDistance;
                 y++) {
                if (!tempRegion->isChunkEmpty(y)) {
                    chunksToRender.push(tempRegion->getChunk(y));
                }
            }
        }
    }
    return chunksToRender;
}

Region*
World::GetRegion(int x, int z)
{
    if (!RegionMap.contains(std::make_pair(x, z))) {
        auto tempRegion = Region(x, z);
        RegionMap.insert(std::make_pair(std::make_pair(x, z), tempRegion));
    }
    return &RegionMap.at(std::make_pair(x, z));
}

}
