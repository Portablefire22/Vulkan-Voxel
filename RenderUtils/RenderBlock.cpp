//
// Created by blakey on 28/11/23.
//

#include "RenderBlock.h"

#include "../main.h"
#include "../vk_engine.h"


Mesh RenderBlock::createHorizontalQuad(float tileSize, FACE face) {
    Mesh quadMesh{};
    quadMesh._vertices.resize(6);
    switch (face) {
        case POSX:
            quadMesh._vertices[0].position = {0,0,0};
            quadMesh._vertices[1].position = {0,1,0};
            quadMesh._vertices[2].position = {1,1,0};

            quadMesh._vertices[3].position = {1,1,0};
            quadMesh._vertices[4].position = {1,0,0};
            quadMesh._vertices[5].position = {0,0,0};

            quadMesh._vertices[0].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[1].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[2].colour = { 0.f,1.f, 0.0f };

            quadMesh._vertices[3].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[4].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[5].colour = { 0.f,1.f, 0.0f }; //pure green
            break;
        case POSZ:
            quadMesh._vertices[0].position = {0,0,0};
            quadMesh._vertices[1].position = {0,1,0};
            quadMesh._vertices[2].position = {0,0,1};
            quadMesh._vertices[3].position = {0,0,1};
            quadMesh._vertices[4].position = {0,1,0};
            quadMesh._vertices[5].position = {0,1,1};

            quadMesh._vertices[0].colour = { 1.f,0.f, 0.0f }; //pure green
            quadMesh._vertices[1].colour = { 1.f,0.f, 0.0f }; //pure green
            quadMesh._vertices[2].colour = { 1.f,0.f, 0.0f };
            quadMesh._vertices[3].colour = { 1.f,0.f, 0.0f }; //pure green
            quadMesh._vertices[4].colour = { 1.f,0.f, 0.0f }; //pure green
            quadMesh._vertices[5].colour = { 1.f,0.f, 0.0f };
            break;
        case NEGX:
            quadMesh._vertices[0].position = {0,0,1};
            quadMesh._vertices[1].position = {0,1,1};
            quadMesh._vertices[2].position = {1,1,1};

            quadMesh._vertices[3].position = {1,1,1};
            quadMesh._vertices[4].position = {1,0,1};
            quadMesh._vertices[5].position = {0,0,1};

            quadMesh._vertices[0].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[1].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[2].colour = { 0.f,1.f, 0.0f };

            quadMesh._vertices[3].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[4].colour = { 0.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[5].colour = { 0.f,1.f, 0.0f }; //pure green
        break;
        case NEGZ:
            quadMesh._vertices[0].position = {1,0,0};
            quadMesh._vertices[1].position = {1,1,0};
            quadMesh._vertices[2].position = {1,0,1};
            quadMesh._vertices[3].position = {1,0,1};
            quadMesh._vertices[4].position = {1,1,0};
            quadMesh._vertices[5].position = {1,1,1};

            quadMesh._vertices[0].colour = { 1.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[1].colour = { 1.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[2].colour = { 1.f,1.f, 0.0f };
            quadMesh._vertices[3].colour = { 1.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[4].colour = { 1.f,1.f, 0.0f }; //pure green
            quadMesh._vertices[5].colour = { 1.f,1.f, 0.0f };
        break;
        case TOP:
            quadMesh._vertices[0].position = {0,1,0};
            quadMesh._vertices[1].position = {0,1,1};
            quadMesh._vertices[2].position = {1,1,1};

            quadMesh._vertices[3].position = {0,1,0};
            quadMesh._vertices[4].position = {1,1,0};
            quadMesh._vertices[5].position = {1,1,1};

            quadMesh._vertices[0].colour = { 1.f,0.f, 1.0f }; //pure green
            quadMesh._vertices[1].colour = { 1.f,0.f, 1.0f }; //pure green
            quadMesh._vertices[2].colour = { 1.f,0.f, 1.0f };
            quadMesh._vertices[3].colour = { 1.f,0.f, 1.0f }; //pure green
            quadMesh._vertices[4].colour = { 1.f,0.f, 1.0f }; //pure green
            quadMesh._vertices[5].colour = { 1.f,0.f, 1.0f };
        break;
        case BOTTOM:
                quadMesh._vertices[0].position = {0,0,0};
            quadMesh._vertices[1].position = {0,0,1};
            quadMesh._vertices[2].position = {1,0,1};

            quadMesh._vertices[3].position = {0,0,0};
            quadMesh._vertices[4].position = {1,0,0};
            quadMesh._vertices[5].position = {1,0,1};


            quadMesh._vertices[0].colour = { 0.f,1.f, 1.0f }; //pure green
            quadMesh._vertices[1].colour = { 0.f,1.f, 1.0f }; //pure green
            quadMesh._vertices[2].colour = { 0.f,1.f, 1.0f };
            quadMesh._vertices[3].colour = { 0.f,1.f, 1.0f }; //pure green
            quadMesh._vertices[4].colour = { 0.f,1.f, 1.0f }; //pure green
            quadMesh._vertices[5].colour = { 0.f,1.f, 1.0f };
            break;

        default:
            break;
    }


    //entryPoint::engine.uploadMesh(quadMesh);
    //entryPoint::engine._meshes["grass"] = quadMesh;
    return quadMesh;
}

bool RenderBlock::createBlocks() {
    Mesh blockMesh{};
    //blockMesh._vertices.resize(36);
    for (int faceVal = POSX; faceVal <= BOTTOM; faceVal++) {
        Mesh tempMesh = createHorizontalQuad(1.0f, static_cast<FACE>(faceVal));
        blockMesh._vertices.insert(blockMesh._vertices.end(), tempMesh._vertices.begin(), tempMesh._vertices.end());
    }
    entryPoint::engine.uploadMesh(blockMesh);
    entryPoint::engine._meshes["grass"] = blockMesh;
    return true;
}
