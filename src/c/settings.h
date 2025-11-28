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

void default_settings(ClaySettings *settings);
void save_settings(ClaySettings *settings);
void load_settings(ClaySettings *settings);