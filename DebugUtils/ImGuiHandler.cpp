//
// Created by blakey on 01/12/23.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "ImGuiHandler.h"

#include <iostream>

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
        ImGui::Text("World Seed: %d", engine.currentWorld.WorldSeed);
        glm::vec3 tempPos = player.getPosition();
        ImGui::Text("Player Position: (%f,%f,%f)", tempPos.x, tempPos.y, tempPos.z);
        ImGui::Text("Current Chunk: (%f,%f,%f)", player.ChunkPosition.x, player.ChunkPosition.y, player.ChunkPosition.z);
        ImGui::Text("Renderables: %d", engine._renderables.size());
        double vm,rss;
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

} // DebugUI