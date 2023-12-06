//
// Created by blakey on 01/12/23.
//

#include "World.h"

#include <cstdlib>
#include <set>
#include <glm/vec4.hpp>
#include "../vk_engine.h"
#include "../RenderUtils/RenderBlock.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_query.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <complex>
#include <iostream>
#include <SDL_timer.h>
#include <unordered_set>

#include "glm/gtx/hash.hpp"
#include <noise/noise.h>


namespace WorldHandler {

    void World::RenderChunks(VulkanEngine& engine, std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction>& chunks) {
        for (chunk::Chunk localChunk : chunks) {
            bool chunkExists = false;
            for (auto t: engine._renderables) {
                if (t.position == localChunk.data.info.ChunkPosition) {
                    chunkExists = true;
                }
            }
            if (chunkExists) {
                continue;
            }

            RenderObject chunkObject;
            std::string name = glm::to_string(localChunk.data.info.ChunkPosition);


            // Check if the chunk mesh already exists, if not then create it
            if (!engine._meshes.contains(name)) {
                localChunk.GenerateChunkMesh();
            }
            chunkObject.mesh = engine.getMesh(name);
            if (chunkObject.mesh == nullptr) {
                continue;
            }
            chunkObject.name = name;


            chunkObject.material = engine.getMaterial("grass");

            glm::mat4 translation = glm::translate(glm::mat4{1.0}, localChunk.data.info.ChunkPosition * (float)CHUNK_SIZE);
            glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3(1,1,1));
            chunkObject.transformMatrix = translation * scale;
            chunkObject.position = localChunk.data.info.ChunkPosition;
            engine._renderables.push_back(chunkObject);

        }
    }

    std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction> World::GetChunksAroundPlayer(VulkanEngine& engine, Player::Player &player, int horzRenderDistance, int vertRenderDistance) {
        std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction> chunksToRender;
        for (int y = player.ChunkPosition.y - vertRenderDistance; y <= player.ChunkPosition.y + vertRenderDistance; y++) {
            for (int z = player.ChunkPosition.z - horzRenderDistance; z <= player.ChunkPosition.z + horzRenderDistance; z++) {
                for (int x = player.ChunkPosition.x - horzRenderDistance; x <= player.ChunkPosition.x + horzRenderDistance; x++) {
                    chunksToRender.insert(GetChunk(engine, x,y,z));
                }
            }
        }
        return chunksToRender;
    }

    chunk::Chunk World::GetChunk(VulkanEngine& engine, int ChunkX, int ChunkY, int ChunkZ) {
        chunk::Chunk localChunk;
        std::map<int32_t, chunk::Chunk> localRegion;
        // Check if the given chunk exists or not
        glm::vec2 chunkVec = {ChunkX, ChunkZ};
        if (WorldMap.contains(chunkVec)) {
            localRegion = WorldMap.at(chunkVec);
            if (localRegion.contains(ChunkY)) {
                return localRegion.at(ChunkY);
            }
            localChunk = engine._ChunkManager.generateChunk({ChunkX, ChunkY, ChunkZ});
            localRegion.insert(std::make_pair(ChunkY, localChunk));
            return localChunk;
        }
        localChunk = engine._ChunkManager.generateChunk({ChunkX, ChunkY, ChunkZ});
        localRegion.insert(std::make_pair(ChunkY, localChunk));
        WorldMap.insert(std::make_pair(chunkVec, localRegion));
        return localChunk;
    }
    void World::GetNoiseHeightMap(glm::vec3&ChunkPos, double NoiseArr[CHUNK_SIZE][CHUNK_SIZE]) {

       // const auto seed = (siv::PerlinNoise::seed_type)this->WorldSeed;
        testI++;
        noise::module::Perlin perlinMod;
        std::hash<std::string> hasher;
        perlinMod.SetSeed(412353523124);
        perlinMod.SetNoiseQuality(noise::QUALITY_BEST);
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                //const double noise = perlin.octave2D_01((x + (int)localChunk.data.info.ChunkPosition.x * CHUNK_SIZE), (z + (int)localChunk.data.info.ChunkPosition.z * CHUNK_SIZE), 128);
                double xNoise =  0.01 * (x + ChunkPos.x * CHUNK_SIZE);
                double yNoise =  1 * (ChunkPos.y * CHUNK_SIZE);
                double zNoise =  0.01 * (z + ChunkPos.z * CHUNK_SIZE);
                auto noise = perlinMod.GetValue(xNoise, yNoise, zNoise);
                NoiseArr[z][x] = round(abs(noise) * CHUNK_SIZE/2);
            }
        }
    }




}