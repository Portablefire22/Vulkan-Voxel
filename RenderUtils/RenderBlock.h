//
// Created by blakey on 28/11/23.
//

#ifndef RENDERBLOCK_H
#define RENDERBLOCK_H
#include "../vk_mesh.h"


namespace RenderBlock {

    enum FACE {
        FRONT = 0,
        RIGHT = 1,
        BACK = 2,
        LEFT = 3,
        TOP = 4,
        BOTTOM = 5
    };

    Mesh createHorizontalQuad(float tileSize, FACE face, glm::vec3 colour = {0.0,0.0,0.0});
    bool createBlocks();

};



#endif
