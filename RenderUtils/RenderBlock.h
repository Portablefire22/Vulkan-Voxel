//
// Created by blakey on 28/11/23.
//

#ifndef RENDERBLOCK_H
#define RENDERBLOCK_H
#include "../vk_mesh.h"


namespace RenderBlock {

    enum FACE {
        POSX = 0,
        POSY = 1,
        POSZ = 2,
        NEGX = 3,
        NEGZ = 4,
        NEGY = 5,
        TOP = 6,
        BOTTOM = 7
    };

    Mesh createHorizontalQuad(float tileSize, FACE face);
    bool createBlocks();

};



#endif
