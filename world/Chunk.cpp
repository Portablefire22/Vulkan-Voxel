//
// Created by blakey on 25/11/23.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "Chunk.h"

#include <SDL2/SDL_timer.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "../main.h"

namespace chunk {

Chunk::Chunk(Region* Parent, glm::vec3 chunkPos)
{
    ChunkPosition = chunkPos;
    ParentRegion = Parent;
}

bool
Chunk::generateChunk()
{
    bool isSurface;      // Stops grass from being places in a cave
    bool isSurfaceBlock; // Stops grass from spawning inside of a mountain
    auto ChunkRegion = entryPoint::engine.currentWorld.GetRegion(
      this->ChunkPosition.x, this->ChunkPosition.z);
    int HEIGHT;
    if (this->ChunkPosition.y * CHUNK_SIZE >= SURFACE_LEVEL) {
        isSurface = true;
    } else {
        isSurface = false;
    }
    int tmpHeight;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            if (isSurface) {
                // HEIGHT = (CHUNK_SIZE/4) * (NoiseArr[x + (z * CHUNK_SIZE)]) +
                // (CHUNK_SIZE / 8);
                tmpHeight = *ChunkRegion->getBlockHeight(x, z);
                if (tmpHeight - abs((int)this->ChunkPosition.y * CHUNK_SIZE) <=
                    0) {
                    HEIGHT = 0;
                } else if (tmpHeight -
                             abs((int)this->ChunkPosition.y * CHUNK_SIZE) >=
                           CHUNK_SIZE) { // If the height is a bit too high then
                                         // cut off at 16
                    HEIGHT = CHUNK_SIZE;
                } else {
                    HEIGHT =
                      tmpHeight - (int)this->ChunkPosition.y * CHUNK_SIZE;
                }
            } else {
                HEIGHT = CHUNK_SIZE;
            }
            for (int y = 0; y < CHUNK_SIZE; y++) {
                if (isSurface) {
                    if (HEIGHT == 0) {
                        break;
                    }
                    if (y < HEIGHT - 3 ||
                        (y + (ChunkPosition.y * CHUNK_SIZE) >= 150 &&
                         y < HEIGHT)) {
                        this->Blocks[y][z][x] = (std::byte)2; // STONE
                        this->ParentRegion->setChunkEmpty(this->ChunkPosition.y,
                                                          false);
                    } else if (y == HEIGHT &&
                               y + (ChunkPosition.y * CHUNK_SIZE) <=
                                 WATER_LEVEL) {
                        this->Blocks[y][z][x] = (std::byte)4; // SAND
                        this->ParentRegion->setChunkEmpty(this->ChunkPosition.y,
                                                          false);
                    } else if (y == HEIGHT - 1) {
                        this->Blocks[y][z][x] = (std::byte)1; // GRASS
                        this->ParentRegion->setChunkEmpty(this->ChunkPosition.y,
                                                          false);
                    } else if (y >= HEIGHT - 4 && y < HEIGHT) {
                        this->Blocks[y][z][x] = (std::byte)3; // DIRT
                        this->ParentRegion->setChunkEmpty(this->ChunkPosition.y,
                                                          false);
                    } else if (y > HEIGHT &&
                               y + (ChunkPosition.y * CHUNK_SIZE) <=
                                 WATER_LEVEL) {
                        this->Blocks[y][z][x] = (std::byte)5; // WATER
                        this->ParentRegion->setChunkEmpty(this->ChunkPosition.y,
                                                          false);
                    } else {
                        break;
                    }
                } else {
                    this->Blocks[y][z][x] = (std::byte)2;
                }
            }
        }
    }
    return true;
}

Mesh
Chunk::GenerateChunkMesh()
{
    Mesh chunkMesh{};
    std::string name = glm::to_string(this->ChunkPosition);
    glm::vec3 blockPosition;
    double t1 = SDL_GetPerformanceCounter();
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                // if (std::all_of(data.Blocks[y], data.Blocks[y] + (CHUNK_SIZE
                // * CHUNK_SIZE), [](bool elem) {return elem == 0;})) break; //
                // Skip all checks if it's just air
                //  If the block is air then just skip
                if (this->Blocks[y][z][x] == (std::byte)0)
                    continue;
                blockPosition = glm::vec3(x, y, z);
                std::vector<RenderBlock::FACE> facesToRender =
                  ShouldRenderFace(blockPosition);
                // If the mesh doesn't exist
                RenderBlock::AddBlockVertices((int)this->Blocks[y][z][x],
                                              chunkMesh,
                                              facesToRender,
                                              blockPosition);
            }
        }
    }
    // if (chunkMesh._vertices.size() > 0) {
    //     entryPoint::engine.uploadMesh(chunkMesh);
    //     entryPoint::engine._meshes[name] = chunkMesh;
    //     double t2 = SDL_GetPerformanceCounter();
    //     double t3 = (double)(t2 - t1) * 1000 / SDL_GetPerformanceFrequency();
    //     std::cout << "Time: " << t3 << std::endl;
    // }
    return chunkMesh;
}

int
Chunk::GetVoxel(glm::vec3& position)
{
    try {
        return static_cast<int>(
          this->Blocks[(int)position.x][(int)position.y][(int)position.z]);
    } catch (std::exception& e) {
        e.what();
        return -1;
    }
}

void
IndexToVec(const int index, glm::vec3* blockPosition)
{
    blockPosition->x = index % CHUNK_SIZE;
    blockPosition->y = floor(index / (CHUNK_SIZE * CHUNK_SIZE));
    blockPosition->z = floor((index % (CHUNK_SIZE * CHUNK_SIZE)) / CHUNK_SIZE);
}

int
VecToIndex(glm::vec3& blockPosition)
{
    return (int)blockPosition.x + ((int)blockPosition.z << 4) +
           ((int)blockPosition.y << 8);
}

std::vector<RenderBlock::FACE>
Chunk::ShouldRenderFace(glm::vec3& originalPos)
{
    std::vector<RenderBlock::FACE> faces;
    for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM;
         faceVal++) {
        int x = originalPos.x;
        int y = originalPos.y;
        int z = originalPos.z;
        bool chunkSide = false;
        // Convert 3D space to an index in the byte array
        switch (static_cast<RenderBlock::FACE>(faceVal)) {
            case RenderBlock::FRONT: // PosX
                x++;
                if (x == CHUNK_SIZE) {
                    if (entryPoint::engine.currentWorld
                          .GetRegion(this->ChunkPosition.x + 1,
                                     this->ChunkPosition.z)
                          ->getChunk(this->ChunkPosition.y)
                          ->Blocks[y][z][0] == static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            case RenderBlock::BACK: // NegX
                x--;
                if (x == -1) {
                    if (entryPoint::engine.currentWorld
                          .GetRegion(this->ChunkPosition.x - 1,
                                     this->ChunkPosition.z)
                          ->getChunk(this->ChunkPosition.y)
                          ->Blocks[y][z][CHUNK_SIZE - 1] ==
                        static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            case RenderBlock::LEFT: // Neg Z
                z--;
                if (z == -1) {
                    if (entryPoint::engine.currentWorld
                          .GetRegion(this->ChunkPosition.x,
                                     this->ChunkPosition.z - 1)
                          ->getChunk(this->ChunkPosition.y)
                          ->Blocks[y][CHUNK_SIZE - 1][x] ==
                        static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            case RenderBlock::RIGHT: // Pos Z
                z++;
                if (z == CHUNK_SIZE) {
                    if (entryPoint::engine.currentWorld
                          .GetRegion(this->ChunkPosition.x,
                                     this->ChunkPosition.z + 1)
                          ->getChunk(this->ChunkPosition.y)
                          ->Blocks[y][0][x] == static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            case RenderBlock::TOP: // Pos Y Works
                y++;
                if (y == CHUNK_SIZE) {
                    if (this->ParentRegion->getChunk(this->ChunkPosition.y + 1)
                          ->Blocks[0][z][x] == static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            case RenderBlock::BOTTOM: // Neg Y
                y--;
                if (y == -1) {
                    if (this->ParentRegion->getChunk(this->ChunkPosition.y - 1)
                          ->Blocks[CHUNK_SIZE - 1][z][x] ==
                        static_cast<std::byte>(0)) {
                        faces.push_back(
                          static_cast<RenderBlock::FACE>(faceVal));
                    }
                    chunkSide = true;
                }
                break;
            default:
                break;
        }
        if (!chunkSide && this->Blocks[y][z][x] == (std::byte)0) {
            faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
        }
    }
    return faces;
}

} // chunk

/* The cubic chunk rendering system can easily cause long load times or lag from
   storing chunks that shouldn't be considered. This will be extremely evident
   when the player can only see 8 chunks above them but are also loading in 8
   chunks below. It can be safe to consider that these chunks need not be
   rendered and as such the chunk culling system should take the player's camera
   into consideration, so only the meshes for the visible chunks are constructed
   / loaded and sent to the GPU

    Look: Frustum culling
    https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
*/
