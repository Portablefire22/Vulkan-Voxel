//
// Created by blakey on 01/12/23.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "ImGuiHandler.h"

#include "../vulkan/vk_engine.h"
#include "../deps/imgui/imgui.h"
#include "../player/Player.h"
#include "../vulkan/vk_engine.h"

namespace DebugUI {
void
PlayerInformation(Player::Player player, VulkanEngine& engine)
{
    ImGui::Begin("World Information");
    ImGui::Text("Current World: %s", engine.currentWorld.WorldName.c_str());
    ImGui::Text("World Seed: %ld", engine.currentWorld.WorldSeed);
    glm::vec3 tempPos = player.getPosition();
    ImGui::Text("Player Position: (%f,%f,%f)", tempPos.x, tempPos.y, tempPos.z);
    ImGui::Text("Current Chunk: (%f,%f,%f)",
                player.ChunkPosition.x,
                player.ChunkPosition.y,
                player.ChunkPosition.z);
    ImGui::Text("Rendered: %ld", engine._renderables.size());
    double vm, rss;
    engine.proccess_mem_usage(vm, rss);
    if (vm <= 1000) {
        ImGui::Text("RAM: %0.2lf KB", vm);
    } else if (vm <= 1000000) {
        ImGui::Text("RAM: %0.2lf MB", vm / 1000.0f);
    } else if (vm <= 1000000000) {
        ImGui::Text("RAM: %0.2lf GB", vm / 1000000.0f);
    } else {
        ImGui::Text("RAM: %0.2lf TB 0_0", vm / 1000000000.0f);
    }
    ImGui::End();
}

void
GameSettings(VulkanEngine& engine)
{
    ImGui::Begin("Game Settings");
    ImGui::SliderInt(
      "Horizontal Render distance", &engine.renderDistanceHorz, 0, 100);
    ImGui::SliderInt(
      "Vertical Render distance", &engine.renderDistanceVert, 0, 100);
    ImGui::Checkbox("Stop chunk loading?", &engine.stopLoadChunks);
    ImGui::End();
}

} // namespace DebugUI
