//
// Created by blakey on 25/11/23.
//

#include "Chunk.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "../main.h"



namespace chunk {


    Chunk::Chunk() {
        data.info.ChunkPosition = {0,0,0};
    }

    Chunk::Chunk(glm::vec3 chunkPos) {
        //std::cout << "Created: " << height << "|" << width << "|" << depth << std::endl;
        //std::cout << "@: " << chunkPos.x << "|" << chunkPos.y << "|" << chunkPos.z << std::endl;
        std::byte blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = { (std::byte) 0};
        data = ChunkData {
            ChunkInfo{
                chunkPos
            },
            blocks[0][0][0],
        };
    }

    Chunk ChunkManager::generateChunk(glm::vec3 ChunkPos) {
        // ChunkPos not currently used but will be required for when noise is used
        Chunk localChunk;
        localChunk.data.info.ChunkPosition = ChunkPos;
        //localChunk.data.Blocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        int HEIGHT;
        double NoiseArr[CHUNK_SIZE][CHUNK_SIZE];
        if (ChunkPos.y >= (128 / CHUNK_SIZE)) {
            entryPoint::engine.currentWorld.GetNoiseHeightMap(ChunkPos, NoiseArr);
        }
        bool topChunk = false;
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (ChunkPos.y >= (128 / CHUNK_SIZE)) {
                    HEIGHT = NoiseArr[z][x];
                    topChunk = true;
                    //std::cout << HEIGHT << std::endl;
                } else {
                    HEIGHT = CHUNK_SIZE;
                }
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    if (HEIGHT != CHUNK_SIZE) {
                        if (y < HEIGHT - 3) {
                            localChunk.data.Blocks[x][y][z] = (std::byte)2;
                        }
                        else if (y == HEIGHT){
                            localChunk.data.Blocks[x][y][z] = (std::byte)1;
                        }
                        else if (y >= HEIGHT - 3 && y < HEIGHT && topChunk) {
                            localChunk.data.Blocks[x][y][z] = (std::byte)3;
                        }
                        else {
                            localChunk.data.Blocks[x][y][z] = (std::byte)0;
                        }
                    } else {
                        localChunk.data.Blocks[x][y][z] = (std::byte)2;
                    }
                }
            }
        }
        /*if (ChunkPos.y >= (50 >> 4)) {
            entryPoint::engine.currentWorld.GetNoiseHeightMap(localChunk, &NoiseVec);
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE ; z++) {
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        if (NoiseVec[x + z << 4] <= y) {
                            localChunk.data.Blocks.push_back((std::byte)1);
                        }
                    }
                }
            }
        } else {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE ; z++) {
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        //Block::Block block = Block::Block(1, glm::vec3{x + ChunkPos.x, y + ChunkPos.y,z + ChunkPos.z }, Block::POSX);
                        localChunk.data.Blocks.push_back((std::byte)1);
                    }
                }
            }
        }*/

        return localChunk;
    }

    Mesh Chunk::GenerateChunkMesh() {
        Mesh chunkMesh{};
        std::string name = glm::to_string(this->data.info.ChunkPosition);
        glm::vec3 blockPosition;
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    std::byte* pBlockId = &this->data.Blocks[x][y][z];
                    // If the block is air then just skip
                    if (*pBlockId == (std::byte)0) continue;
                    blockPosition = glm::vec3(x,y,z);
                    std::vector<RenderBlock::FACE> facesToRender = ShouldRenderFace(blockPosition);
                    // If the mesh doesn't exist
                    RenderBlock::AddBlockVertices((int)this->data.Blocks[x][y][z], chunkMesh, facesToRender, blockPosition);
                }
            }
        }
        if (chunkMesh._vertices.size() > 0) {
            entryPoint::engine.uploadMesh(chunkMesh);
            entryPoint::engine._meshes[name] = chunkMesh;
        }
        return chunkMesh;
    }

    int Chunk::GetVoxel(glm::vec3& position) {
        try {
            return static_cast<int>(this->data.Blocks[(int)position.x][(int)position.y][(int)position.z]);
        } catch (std::exception e) {
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
        glm::vec3 tempPos;

        for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM; faceVal++) {
            tempPos = originalPos;
            bool chunkSide = false;
            // Convert 3D space to an index in the byte array
            switch (static_cast<RenderBlock::FACE>(faceVal)) {
                case RenderBlock::FRONT: // PosX
                    tempPos = tempPos + glm::vec3{1,0,0};
                    if (tempPos.x == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::BACK: // NegX
                    tempPos = tempPos + glm::vec3{-1,0,0};
                    if (tempPos.x == -1) chunkSide = true;
                    break;
                case RenderBlock::LEFT: // Neg Z
                    tempPos = tempPos + glm::vec3{0,0,-1};
                    if (tempPos.z == -1) chunkSide = true;
                    break;
                case RenderBlock::RIGHT: // Pos Z
                    tempPos = tempPos + glm::vec3{0,0,1};
                    if (tempPos.z == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::TOP: // Pos Y Works
                    tempPos = tempPos + glm::vec3{0,1,0};
                    if (tempPos.y == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::BOTTOM: // Neg Y
                    tempPos = tempPos + glm::vec3{0,-1,0};
                    if (tempPos.y == -1) chunkSide = true;
                    break;
                default:
                    break;
            }
            if (chunkSide) {
                faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
            } else if (GetVoxel(tempPos) == 0) {
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