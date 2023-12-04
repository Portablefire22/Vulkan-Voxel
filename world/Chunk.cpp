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
        std::vector<std::byte> blocks;
        data = ChunkData {
            ChunkInfo{
                chunkPos
            },
            blocks,
        };
    }

    Chunk ChunkManager::generateChunk(glm::vec3 ChunkPos) {
        // ChunkPos not currently used but will be required for when noise is used
        Chunk localChunk;
        localChunk.data.info.ChunkPosition = ChunkPos;
        //localChunk.data.Blocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE ; z++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    //Block::Block block = Block::Block(1, glm::vec3{x + ChunkPos.x, y + ChunkPos.y,z + ChunkPos.z }, Block::POSX);
                    localChunk.data.Blocks.push_back((std::byte)1);
                }
            }
        }
        return localChunk;
    }

    Mesh Chunk::GenerateChunkMesh() {
        Mesh chunkMesh{};
        std::string name = glm::to_string(this->data.info.ChunkPosition);
        glm::vec3 blockPosition;
        for (int i = 0; i < this->data.Blocks.size(); i++) {
            std::byte* pBlockId = &this->data.Blocks[i];
            // If the block is air then just skip
            if (*pBlockId == (std::byte)0) continue;
            IndexToVec(i, &blockPosition);
            std::vector<RenderBlock::FACE> facesToRender = ShouldRenderFace(blockPosition);
            // If the mesh doesn't exist
            RenderBlock::AddBlockVertices(chunkMesh, facesToRender, blockPosition);
        }
        entryPoint::engine.uploadMesh(chunkMesh);
        entryPoint::engine._meshes[name] = chunkMesh;
        return chunkMesh;
    }

    int Chunk::GetVoxel(glm::vec3& position) {
        return static_cast<int>(this->data.Blocks[(int)position.x + ((int)position.z << 4) + ((int)position.y << 8)]);
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
        glm::vec3 tempPos = originalPos;
        for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM; faceVal++) {
            bool chunkSide = false;
            // Convert 3D space to an index in the byte array
            switch (static_cast<RenderBlock::FACE>(faceVal)) {
                case RenderBlock::FRONT: // PosX
                    tempPos = tempPos + glm::vec3{1,0,0};
                    if (tempPos.x == CHUNK_SIZE) chunkSide = true;
                    break;
                case RenderBlock::BACK: // NegX
                    tempPos = tempPos + glm::vec3{-1,0,0};
                    if (tempPos.x == 0) chunkSide = true;
                    break;
                case RenderBlock::LEFT: // Neg Z
                    tempPos = tempPos + glm::vec3{0,0,-1};
                    if (tempPos.z == 0) chunkSide = true;
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
                    if (tempPos.y == 0) chunkSide = true;
                    break;
                default:
                    break;
            }
            if (GetVoxel(tempPos) != 1 || chunkSide) {
                faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
            }
        }
        return faces;
    }

} // chunk