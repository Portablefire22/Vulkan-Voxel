//
// Created by blakey on 01/12/23.
//

#include "ImGuiHandler.h"

#include "../vk_engine.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_vulkan.h"
#include "../imgui/imgui_internal.h"
#include "../player/Player.h"

namespace DebugUI {
    void PlayerInformation(Player::Player player, VulkanEngine& engine) {
        ImGui::Begin("Player Information");
        ImGui::Text("Current World: %s", engine.currentWorld.WorldName);
        ImGui::Text("Player Position: (%f,%f,%f)", player.Position.x, player.Position.y, player.Position.z);
        ImGui::Text("Current Chunk: (%f,%f,%f)", player.ChunkPosition.x, player.ChunkPosition.y, player.ChunkPosition.z);
        ImGui::Text("Renderables: %d", engine._renderables.size());
        ImGui::End();
    }

} // DebugUI