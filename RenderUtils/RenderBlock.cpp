//
// Created by blakey on 28/11/23.
//

#include "RenderBlock.h"

#include <iostream>
#include <ostream>

#include "../main.h"
#include "../vk_engine.h"


Mesh RenderBlock::createHorizontalQuad(int blockId, float tileSize, FACE& face,glm::vec3& startPos, glm::vec3 colour) {
    Mesh quadMesh{};
    quadMesh._vertices.resize(6);
    switch (face) {
        case LEFT:
            quadMesh._vertices[0].position = {startPos.x,startPos.y,startPos.z};
            quadMesh._vertices[1].position = {startPos.x,startPos.y + tileSize,startPos.z};
            quadMesh._vertices[2].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z};

            quadMesh._vertices[3].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z};
            quadMesh._vertices[4].position = {startPos.x + tileSize,startPos.y,startPos.z};
            quadMesh._vertices[5].position = {startPos.x,startPos.y,startPos.z};
            break;
        case BACK:
            quadMesh._vertices[0].position = {startPos.x,startPos.y,startPos.z};
            quadMesh._vertices[1].position = {startPos.x,startPos.y + tileSize,startPos.z};
            quadMesh._vertices[2].position = {startPos.x,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[3].position = {startPos.x,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[4].position = {startPos.x,startPos.y,startPos.z + tileSize};
            quadMesh._vertices[5].position = {startPos.x,startPos.y,startPos.z};
            break;
        case RIGHT:
            quadMesh._vertices[0].position = {startPos.x,startPos.y,startPos.z + tileSize};
            quadMesh._vertices[1].position = {startPos.x,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[2].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};

            quadMesh._vertices[3].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[4].position = {startPos.x + tileSize,startPos.y,startPos.z + tileSize};
            quadMesh._vertices[5].position = {startPos.x,startPos.y,startPos.z + tileSize};
        break;
        case FRONT:
            quadMesh._vertices[0].position = {startPos.x + tileSize,startPos.y,startPos.z};
            quadMesh._vertices[1].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z};
            quadMesh._vertices[2].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[3].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[4].position = {startPos.x + tileSize,startPos.y,startPos.z + tileSize};
            quadMesh._vertices[5].position = {startPos.x + tileSize,startPos.y,startPos.z};
        break;
        case TOP:
            quadMesh._vertices[0].position = {startPos.x,startPos.y + tileSize,startPos.z};
            quadMesh._vertices[1].position = {startPos.x,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[2].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};

            quadMesh._vertices[3].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z + tileSize};
            quadMesh._vertices[4].position = {startPos.x + tileSize,startPos.y + tileSize,startPos.z};

            quadMesh._vertices[5].position = {startPos.x,startPos.y + tileSize,startPos.z};
        break;
        case BOTTOM:
                quadMesh._vertices[0].position = {startPos.x,startPos.y,startPos.z};
                quadMesh._vertices[1].position = {startPos.x,startPos.y,startPos.z + tileSize};
                quadMesh._vertices[2].position = {startPos.x + tileSize,startPos.y,startPos.z + tileSize};

                quadMesh._vertices[3].position = {startPos.x + tileSize,startPos.y,startPos.z + tileSize};
                quadMesh._vertices[4].position = {startPos.x + tileSize,startPos.y,startPos.z};
                quadMesh._vertices[5].position = {startPos.x,startPos.y,startPos.z};
            break;

        default:
            break;
    }
    double uvEnd =  ((blockId) * 16) / (128);
    double uvStart = ((blockId - tileSize) * 16) / (128);

    double xMin = (double)((blockId - tileSize) * 16) / 128;
    double xMax = (double)(blockId * 16) / 128;
    double yMin = (double)((blockId / 8) * 16) / 128;
    double yMax = (double)(16 + ((blockId / 8) * 16)) / 128;

    //std::cout << "BLOCK ID: " << blockId << std::endl << xMin << " | " << xMax << std::endl << yMin << " | " << yMax << std::endl;


    quadMesh._vertices[0].colour = colour;
    quadMesh._vertices[1].colour = colour;
    quadMesh._vertices[2].colour = colour;

    quadMesh._vertices[3].colour = colour;
    quadMesh._vertices[4].colour = colour;
    quadMesh._vertices[5].colour = colour;

    /*quadMesh._vertices[0].uv = {uvStart,uvStart};
    quadMesh._vertices[1].uv = {uvStart,uvEnd};
    quadMesh._vertices[2].uv = {uvEnd,uvEnd};
    quadMesh._vertices[3].uv = {uvEnd,uvEnd};
    quadMesh._vertices[4].uv = {uvEnd,uvStart};
    quadMesh._vertices[5].uv = {uvStart,uvStart};*/
    quadMesh._vertices[0].uv = {xMin,yMin};
    quadMesh._vertices[1].uv = {xMin,yMax};
    quadMesh._vertices[2].uv = {xMax,yMax};
    quadMesh._vertices[3].uv = {xMax,yMax};
    quadMesh._vertices[4].uv = {xMax,yMin};
    quadMesh._vertices[5].uv = {xMin,yMin};


    //entryPoint::engine.uploadMesh(quadMesh);
    //entryPoint::engine._meshes["grass"] = quadMesh;
    return quadMesh;
}

bool RenderBlock::createBlocks() {
    Mesh blockMesh{};
    //blockMesh._vertices.resize(36);
    for (int faceVal = FRONT; faceVal <= BOTTOM; faceVal++) {
        double r = ((double) rand() / (RAND_MAX)) + 1;
        glm::vec3 colour = {r,r,r};
        //Mesh tempMesh = createHorizontalQuad(1.f, static_cast<FACE>(faceVal), &colour);
        //blockMesh._vertices.insert(blockMesh._vertices.end(), tempMesh._vertices.begin(), tempMesh._vertices.end());
    }
    entryPoint::engine.uploadMesh(blockMesh);
    entryPoint::engine._meshes["grass"] = blockMesh;
    return true;
}

void RenderBlock::AddBlockVertices(int blockId, Mesh&chunkMesh, std::vector<FACE>& faces, glm::vec3& Position) {
    // Only add the faces that were given
    for (FACE face : faces) {
        Mesh tempMesh = createHorizontalQuad(blockId, 1.f, face, Position, {1,1,1}); // Colour currently goes unused
        chunkMesh._vertices.insert(chunkMesh._vertices.end(), tempMesh._vertices.begin(), tempMesh._vertices.end());
    }

}
