#include "settings.h"

void default_settings(ClaySettings *settings) {
    // Colors
    settings->BackgroundColor = GColorWhite;
    settings->ForegroundColor = GColorBlack;

    // Logo
    settings->ShowLogo = true;
    settings->InvertLogoStateOnDisconnect = true;

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