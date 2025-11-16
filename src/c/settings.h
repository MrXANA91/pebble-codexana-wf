#pragma once

#include <pebble.h>

#define SETTINGS_KEY    1

typedef struct {
    GColor BackgroundColor;
    GColor ForegroundColor;
    bool ShowLogo;
    bool InvertLogoStateOnDisconnect;
    bool VibrateOnDisconnect;
} ClaySettings;

void default_settings(ClaySettings *settings);
void save_settings(ClaySettings *settings);
void load_settings(ClaySettings *settings);