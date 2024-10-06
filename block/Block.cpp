// blockVertices
//  Created by blakey on 26/11/23.
//

#include "Block.h"

namespace Block {
BlockState blockState{};
Block::Block(int blockId, glm::vec3 position, DIRECTION facing)
{
    blockState.BlockId = blockId;
    blockState.Position = position;
    blockState.Facing = facing;
    // mainGame::createBlockVertex();
}
} // Block