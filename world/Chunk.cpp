//
// Created by blakey on 25/11/23.
//

#include "Chunk.h"

#include <iostream>
#include <SDL_timer.h>
#include <glm/gtx/string_cast.hpp>

#include "../main.h"



namespace chunk {

    Chunk::Chunk(glm::vec3 chunkPos) {
        std::byte blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = { (std::byte) 0};
        data = ChunkData {
            chunkPos,
            blocks[0][0][0],
        };
    }

    bool Chunk::generateChunk() {
        bool isSurface; // Stops grass from being places in a cave
        bool isSurfaceBlock; // Stops grass from spawning inside of a mountain
        auto ChunkRegion = entryPoint::engine.currentWorld.GetRegion(this->data.ChunkPosition.x, this->data.ChunkPosition.z);
        int HEIGHT;
        if (this->data.ChunkPosition.y * CHUNK_SIZE >= SURFACE_LEVEL) {
            isSurface = true;
        }
        else {
            isSurface = false;
        }
        int tmpHeight;
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (isSurface) {
                    //HEIGHT = (CHUNK_SIZE/4) * (NoiseArr[x + (z * CHUNK_SIZE)]) + (CHUNK_SIZE / 8);
                    tmpHeight = *ChunkRegion->getBlockHeight(x,z);
                    if (tmpHeight - abs((int)this->data.ChunkPosition.y * CHUNK_SIZE) <= 0) { HEIGHT = 0;}
                    else if (tmpHeight - abs((int)this->data.ChunkPosition.y * CHUNK_SIZE) >= CHUNK_SIZE ) { // If the height is a bit too high then cut off at 16
                        HEIGHT = CHUNK_SIZE;
                    } else {
                        HEIGHT = tmpHeight - (int)this->data.ChunkPosition.y * CHUNK_SIZE;
                    }
                } else {
                    HEIGHT = CHUNK_SIZE;
                }
                for (int y = 0; y < HEIGHT; y++) {
                    if (isSurface) {
                        if (y < HEIGHT - 3) {
                            this->data.Blocks[y][z][x] = (std::byte)2; // STONE
                        }
                        else if (y == HEIGHT && y * CHUNK_SIZE <= WATER_LEVEL) {
                            this->data.Blocks[y][z][x] = (std::byte)4; // SAND
                        }
                        else if (y == HEIGHT-1){
                            this->data.Blocks[y][z][x] = (std::byte)1; // GRASS
                        }
                        else if (y >= HEIGHT - 4 && y < HEIGHT) {
                            this->data.Blocks[y][z][x] = (std::byte)3; // DIRT
                        }
                        else if (y > HEIGHT && y * CHUNK_SIZE <= WATER_LEVEL) {
                            this->data.Blocks[y][z][x] = (std::byte)5; // WATER
                        }
                        else {
                            this->data.Blocks[y][z][x] = (std::byte)0; // AIR
                        }
                    } else {
                        this->data.Blocks[y][z][x] = (std::byte)2;
                    }
                }
            }
        }
        return true;
    }

    Mesh Chunk::GenerateChunkMesh() {
        Mesh chunkMesh{};
        std::string name = glm::to_string(this->data.ChunkPosition);
        glm::vec3 blockPosition;
        double t1 = SDL_GetPerformanceCounter();
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    //if (std::all_of(data.Blocks[y], data.Blocks[y] + (CHUNK_SIZE * CHUNK_SIZE), [](bool elem) {return elem == 0;})) break; // Skip all checks if it's just air
                    // If the block is air then just skip
                    if (this->data.Blocks[y][z][x] == (std::byte)0) continue;
                    blockPosition = glm::vec3(x,y,z);
                    std::vector<RenderBlock::FACE> facesToRender = ShouldRenderFace(blockPosition);
                    // If the mesh doesn't exist
                    RenderBlock::AddBlockVertices((int)this->data.Blocks[y][z][x], chunkMesh, facesToRender, blockPosition);
                }
            }
        }
        if (chunkMesh._vertices.size() > 0) {
            entryPoint::engine.uploadMesh(chunkMesh);
            entryPoint::engine._meshes[name] = chunkMesh;
            double t2 = SDL_GetPerformanceCounter();
            double t3 = (double)(t2 - t1) * 1000 / SDL_GetPerformanceFrequency();
            std::cout << "Time: " << t3 << std::endl;
        }
        return chunkMesh;
    }

    int Chunk::GetVoxel(glm::vec3& position) {
        try {
            return static_cast<int>(this->data.Blocks[(int)position.x][(int)position.y][(int)position.z]);
        } catch (std::exception& e) {
            e.what();
        }
    }

    void IndexToVec(const int index, glm::vec3* blockPosition) {
        blockPosition->x = index % CHUNK_SIZE;
        blockPosition->y = floor(index / (CHUNK_SIZE * CHUNK_SIZE));
        blockPosition->z = floor((index % (CHUNK_SIZE * CHUNK_SIZE)) / CHUNK_SIZE);
    }

    int VecToIndex(glm::vec3& blockPosition) {
        return (int)blockPosition.x + ((int)blockPosition.z << 4) + ((int)blockPosition.y << 8);
    }

    std::vector<RenderBlock::FACE> Chunk::ShouldRenderFace(glm::vec3& originalPos) {
        std::vector<RenderBlock::FACE> faces;
        for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM; faceVal++) {
            int x = originalPos.x;
            int y = originalPos.y;
            int z = originalPos.z;
            bool chunkSide = false;
            // Convert 3D space to an index in the byte array
            switch (static_cast<RenderBlock::FACE>(faceVal)) {
                case RenderBlock::FRONT: // PosX
                    x++;
                    if (x == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::BACK: // NegX
                    x--;
                    if (x == -1) chunkSide = true;
                    break;
                case RenderBlock::LEFT: // Neg Z
                    z--;
                    if (z == -1) chunkSide = true;
                    break;
                case RenderBlock::RIGHT: // Pos Z
                    z++;
                    if (z == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::TOP: // Pos Y Works
                    y++;
                    if (y == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::BOTTOM: // Neg Y
                    y--;
                    if (y == -1) chunkSide = true;
                    break;
                default:
                    break;
            }

            if (chunkSide) {
                faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
            } else if (this->data.Blocks[y][z][x] == (std::byte)0) {
                faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
            }
        }
        return faces;
    }

} // chunk


/* The cubic chunk rendering system can easily cause long load times or lag from storing chunks that shouldn't be considered.
    This will be extremely evident when the player can only see 8 chunks above them but are also loading in 8 chunks below.
    It can be safe to consider that these chunks need not be rendered and as such the chunk culling system should take the player's camera into consideration,
    so only the meshes for the visible chunks are constructed / loaded and sent to the GPU

    Look: Frustum culling
    https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
*/