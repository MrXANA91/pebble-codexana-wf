#pragma once

#include <pebble.h>

#define SETTINGS_KEY    1

#define SETTINGS_DEFAULT_BackgroundColor             GColorWhite
#define SETTINGS_DEFAULT_TextColor                   GColorBlack
#define SETTINGS_DEFAULT_EyeColor                    PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorBlack)
#define SETTINGS_DEFAULT_EyeGrayedColor              GColorLightGray
#define SETTINGS_DEFAULT_BatteryLevelColor           PBL_IF_COLOR_ELSE(GColorBulgarianRose, GColorDarkGray)
#define SETTINGS_DEFAULT_StepCounterColor            PBL_IF_COLOR_ELSE(GColorOxfordBlue, GColorDarkGray)
#define SETTINGS_DEFAULT_EyeOnConnected              EYE_MODIFIER_NONE
#define SETTINGS_DEFAULT_EyeOnDisconnected           EYE_MODIFIER_VOID
#define SETTINGS_DEFAULT_DisplayBatteryPercentage    true
#define SETTINGS_DEFAULT_DisplayBatteryBar           true
#define SETTINGS_DEFAULT_DisplayStepsCounter         true
#define SETTINGS_DEFAULT_DisplayStepsBar             true
#define SETTINGS_DEFAULT_StepsBarMax                 5000
#define SETTINGS_DEFAULT_VibrateOnDisconnect         false

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
    GColor StepCounterColor;
    EyeModifiersEnum_t EyeOnConnected;
    EyeModifiersEnum_t EyeOnDisconnected;
    bool DisplayBatteryPercentage;
    bool DisplayBatteryBar;
    bool DisplayStepsCounter;
    bool DisplayStepsBar;
    int StepsBarMax;
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