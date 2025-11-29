#pragma once

#include <pebble.h>

#define SETTINGS_KEY    1

typedef enum {
    EYE_MODIFIER_NONE = 0,
    EYE_MODIFIER_HIDE = 0x01,
    EYE_MODIFIER_VOID = 0x02,
    EYE_MODIFIER_GRAY = 0x04,
} EyeModifiersEnum_t;

typedef struct {
    GColor BackgroundColor;
    GColor TextColor;
    GColor EyeColor;
    GColor EyeGrayedColor;
    GColor BatteryLevelColor;
    EyeModifiersEnum_t EyeOnConnected;
    EyeModifiersEnum_t EyeOnDisconnected;
    bool DisplayBatteryPercentage;
    bool DisplayBatteryBar;
    bool VibrateOnDisconnect;

} ClaySettings;

typedef void (*NewSettingsHandler)(ClaySettings *);

/// @brief Initialize settings
void settings_init();
/// @brief Gets a pointer to the settings
/// @return Pointer to the ClaySettings struct
ClaySettings* settings_get();
/// @brief Subscribe to be notified when new settings are being loaded
/// @param handler Handler to be called when new settings are loaded
void settings_new_subscribe(NewSettingsHandler);