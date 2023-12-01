//
// Created by blakey on 01/12/23.
//

#ifndef IMGUIHANDLER_H
#define IMGUIHANDLER_H
//#include "../vk_engine.h"
#include "../player/Player.h"

class VulkanEngine;

namespace DebugUI {
    void PlayerInformation(Player::Player player, VulkanEngine&engine);
} // DebugUI

#endif //IMGUIHANDLER_H
