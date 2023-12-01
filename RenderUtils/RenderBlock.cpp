//
// Created by blakey on 28/11/23.
//

#include "RenderBlock.h"

#include "../main.h"
#include "../vk_engine.h"


Mesh RenderBlock::createHorizontalQuad(float tileSize, FACE face, glm::vec3 colour) {
    Mesh quadMesh{};
    quadMesh._vertices.resize(6);
    switch (face) {
        case FRONT:
            quadMesh._vertices[0].position = {0,0,0};
            quadMesh._vertices[1].position = {0,tileSize,0};
            quadMesh._vertices[2].position = {tileSize,tileSize,0};

            quadMesh._vertices[3].position = {tileSize,tileSize,0};
            quadMesh._vertices[4].position = {tileSize,0,0};
            quadMesh._vertices[5].position = {0,0,0};
            break;
        case RIGHT:
            quadMesh._vertices[0].position = {0,0,0};
            quadMesh._vertices[1].position = {0,tileSize,0};
            quadMesh._vertices[2].position = {0,tileSize,tileSize};
            quadMesh._vertices[3].position = {0,tileSize,tileSize};
            quadMesh._vertices[4].position = {0,0,tileSize};
            quadMesh._vertices[5].position = {0,0,0};
            break;
        case BACK:
            quadMesh._vertices[0].position = {0,0,tileSize};
            quadMesh._vertices[1].position = {0,tileSize,tileSize};
            quadMesh._vertices[2].position = {tileSize,tileSize,tileSize};

            quadMesh._vertices[3].position = {tileSize,tileSize,tileSize};
            quadMesh._vertices[4].position = {tileSize,0,tileSize};
            quadMesh._vertices[5].position = {0,0,tileSize};
        break;
        case LEFT:
            quadMesh._vertices[0].position = {tileSize,0,0};
            quadMesh._vertices[1].position = {tileSize,tileSize,0};
            quadMesh._vertices[2].position = {tileSize,tileSize,tileSize};
            quadMesh._vertices[3].position = {tileSize,tileSize,tileSize};
            quadMesh._vertices[4].position = {tileSize,0,tileSize};
            quadMesh._vertices[5].position = {tileSize,0,0};
        break;
        case TOP:
            quadMesh._vertices[0].position = {0,tileSize,0};
            quadMesh._vertices[1].position = {0,tileSize,tileSize};
            quadMesh._vertices[2].position = {tileSize,tileSize,tileSize};

            quadMesh._vertices[3].position = {0,tileSize,0};
            quadMesh._vertices[4].position = {tileSize,tileSize,0};
            quadMesh._vertices[5].position = {tileSize,tileSize,tileSize};
        break;
        case BOTTOM:
                quadMesh._vertices[0].position = {0,0,0};
                quadMesh._vertices[1].position = {0,0,tileSize};
                quadMesh._vertices[2].position = {tileSize,0,tileSize};

                quadMesh._vertices[3].position = {0,0,0};
                quadMesh._vertices[4].position = {tileSize,0,0};
                quadMesh._vertices[5].position = {tileSize,0,tileSize};
            break;

        default:
            break;
    }
    quadMesh._vertices[0].colour = colour;
    quadMesh._vertices[1].colour = colour;
    quadMesh._vertices[2].colour = colour;

    quadMesh._vertices[3].colour = colour;
    quadMesh._vertices[4].colour = colour;
    quadMesh._vertices[5].colour = colour;

    quadMesh._vertices[0].uv = {0,0};
    quadMesh._vertices[1].uv = {0,tileSize};
    quadMesh._vertices[2].uv = {tileSize,tileSize};
    quadMesh._vertices[3].uv = {tileSize,tileSize};
    quadMesh._vertices[4].uv = {tileSize,0};
    quadMesh._vertices[5].uv = {0,0};


    //entryPoint::engine.uploadMesh(quadMesh);
    //entryPoint::engine._meshes["grass"] = quadMesh;
    return quadMesh;
}

bool RenderBlock::createBlocks() {
    Mesh blockMesh{};
    //blockMesh._vertices.resize(36);
    for (int faceVal = FRONT; faceVal <= BOTTOM; faceVal++) {
        double r = ((double) rand() / (RAND_MAX)) + 1;
        Mesh tempMesh = createHorizontalQuad(1.f, static_cast<FACE>(faceVal), {r,r,r});
        blockMesh._vertices.insert(blockMesh._vertices.end(), tempMesh._vertices.begin(), tempMesh._vertices.end());
    }
    entryPoint::engine.uploadMesh(blockMesh);
    entryPoint::engine._meshes["grass"] = blockMesh;
    return true;
}

void RenderBlock::AddBlockVertices(Mesh&chunkMesh, std::vector<FACE> faces) {
    // Only add the faces that were given
    for (FACE face : faces) {
        Mesh tempMesh = createHorizontalQuad(1.f, face, {1,1,1}); // Colour currently goes unused
        chunkMesh._vertices.insert(chunkMesh._vertices.end(), tempMesh._vertices.begin(), tempMesh._vertices.end());
    }
}
