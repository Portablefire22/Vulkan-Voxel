//
// Created by blakey on 26/11/23.
//
#pragma once
#include <glm/vec3.hpp>

namespace Block {

enum DIRECTION
{
    POSX,
    POSY,
    POSZ,
    NEGX,
    NEGY,
    NEGZ
};

struct BlockState
{
    int BlockId;
    glm::vec3 Position;
    DIRECTION Facing;
};

class Block
{

  public:
    BlockState blockState;
    Block(int blockId, glm::vec3 position, DIRECTION facing);
};

} // Block
