#include "settings.h"

void default_settings(ClaySettings *settings) {
    // Colors
    settings->BackgroundColor = GColorWhite;
    settings->TextColor = GColorBlack;
    settings->EyeColor = PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorBlack);
    settings->EyeGrayedColor = GColorLightGray;

    // XANA Eye
    settings->EyeOnConnected = EYE_MODIFIER_NONE;
    settings->EyeOnDisconnected = EYE_MODIFIER_VOID;

    // Misc
    settings->VibrateOnDisconnect = false;
}

void save_settings(ClaySettings *settings) {
    persist_write_data(SETTINGS_KEY, settings, sizeof(ClaySettings));
}

void load_settings(ClaySettings *settings) {
    default_settings(settings);

    if (persist_exists(SETTINGS_KEY)) {
        persist_read_data(SETTINGS_KEY, settings, sizeof(ClaySettings));
    }
}