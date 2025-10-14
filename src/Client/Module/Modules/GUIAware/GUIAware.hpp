#pragma once

#include "Module.hpp"
#include "Module/SDK/SDK.hpp"

class GUIAware : public Module {
public:
    GUIAware() : Module("GUIAware", "Camera zooms above player when a Chest opens", Category::RENDER) {}

    // Module lifecycle
    void onEnable() override;
    void onDisable() override;
    void defaultConfig() override;

    // Event handlers
    void onScreenOpen(ScreenOpenEvent &event);
    void onScreenClose(ScreenCloseEvent &event);
    void onTick(TickEvent &event);
    void settingsRender(float settingsOffset) override;

private:
    bool isActive = false;       // whether zoom is currently active
    bool resetting = false;      // whether smooth reset is running
    float currentOffset = 0.0f;  // current smooth zoom height
    float cameraHeight = 4.0f;   // example default value
    float smoothSpeed = 6.0f;    // example default value
};