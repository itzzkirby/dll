#include "GUIAware.hpp"
#include "Freelook/Freelook.hpp" // Corrected include path
#include "SDK/SDK.hpp"
#include <algorithm> // for std::clamp
#include <cmath>     // for abs

// module cycle
void GUIAware::onEnable() {
    Listen(this, TickEvent, &GUIAware::onTick);
    FlarialGUI::Notify("GUIAware enabled. Camera will zoom above player when GUI opens.");
    Module::onEnable();
}

void GUIAware::onDisable() {
    Deafen(this, TickEvent, &GUIAware::onTick);

    // Correct: call unpatch on a FreeLook instance, not statically
    if (SDK::clientInstance && SDK::clientInstance->getModuleManager()) {
        auto freelook = SDK::clientInstance->getModuleManager()->get<FreeLook>();
        if (freelook) {
            freelook->unpatch();
        }
    }

    isActive = false;
    currentOffset = 0.0f;
    Module::onDisable();
}

void GUIAware::defaultConfig() {
    Module::defaultConfig("core");
    setDef("cameraHeight", cameraHeight);
    setDef("smoothSpeed", smoothSpeed);
}

// tick
void GUIAware::onTick(TickEvent& event) {
    // Cache localPlayer and camera pointers at the start of the tick
    auto clientInstance = SDK::clientInstance;
    auto player = clientInstance ? clientInstance->getLocalPlayer() : nullptr;
    auto camera = clientInstance ? clientInstance->getCamera() : nullptr;
    if (!player || !camera) return;

    //activate?
    if (SDK::getCurrentScreen() == "chest_screen") {
        isActive = true;
    } else {
        isActive = false;
    }

    // === Update camera smoothly ===
    float targetOffset = isActive ? cameraHeight : 0.0f;
    currentOffset += (targetOffset - currentOffset) * smoothSpeed * event.deltaTime;

    // Update camera position
    camera->setPosition(
        player->getPos().x,
        player->getPos().y + currentOffset,
        player->getPos().z
    );

    // Smoothly interpolate pitch
    float targetPitch = isActive ? 90.0f : player->getPitch();
    float camPitch = camera->getPitch();
    camPitch += (targetPitch - camPitch) * smoothSpeed * event.deltaTime;
    camera->setPitch(camPitch);

    // Smoothly align yaw with player
    float camYaw = camera->getYaw();
    camYaw += (player->getYaw() - camYaw) * smoothSpeed * event.deltaTime;
    camera->setYaw(camYaw);
}

void GUIAware::settingsRender(float settingsOffset) {
    ImGui::SliderFloat("Camera Height", &cameraHeight, 1.0f, 10.0f, "%.1f");
    ImGui::SliderFloat("Smooth Speed", &smoothSpeed, 1.0f, 20.0f, "%.1f");
}