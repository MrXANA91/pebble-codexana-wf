#include "settings.h"

static ClaySettings s_settings;
static NewSettingsHandler s_new_settings_handler;

static void default_settings() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Default settings");

    // Colors
    s_settings.BackgroundColor = SETTINGS_DEFAULT_BackgroundColor;
    s_settings.TextColor = SETTINGS_DEFAULT_TextColor;
    s_settings.EyeColor = SETTINGS_DEFAULT_EyeColor;
    s_settings.EyeGrayedColor = SETTINGS_DEFAULT_EyeGrayedColor;
    s_settings.BatteryLevelColor = SETTINGS_DEFAULT_BatteryLevelColor;
    s_settings.StepCounterColor = SETTINGS_DEFAULT_StepCounterColor;

    // XANA Eye
    s_settings.EyeOnConnected = SETTINGS_DEFAULT_EyeOnConnected;
    s_settings.EyeOnDisconnected = SETTINGS_DEFAULT_EyeOnDisconnected;

    // Battery
    s_settings.DisplayBatteryPercentage = SETTINGS_DEFAULT_DisplayBatteryPercentage;
    s_settings.DisplayBatteryBar = SETTINGS_DEFAULT_DisplayBatteryBar;

    // Steps
    s_settings.DisplayStepsCounter = SETTINGS_DEFAULT_DisplayStepsCounter;
    s_settings.DisplayStepsBar = SETTINGS_DEFAULT_DisplayStepsBar;
    s_settings.StepsBarMax = SETTINGS_DEFAULT_StepsBarMax;

    // Misc
    s_settings.VibrateOnDisconnect = SETTINGS_DEFAULT_VibrateOnDisconnect;
}

static void save_settings() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Saving settings");
    persist_write_data(SETTINGS_KEY, &s_settings, sizeof(ClaySettings));
}

static void load_settings() {
    default_settings();

    if (persist_exists(SETTINGS_KEY)) {
        APP_LOG(APP_LOG_LEVEL_INFO, "Loading settings");
        persist_read_data(SETTINGS_KEY, &s_settings, sizeof(ClaySettings));
    } else {
        APP_LOG(APP_LOG_LEVEL_INFO, "No existing settings loaded");
    }
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Colors
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    s_settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  Tuple *txt_color_t = dict_find(iter, MESSAGE_KEY_TextColor);
  if(txt_color_t) {
    s_settings.TextColor = GColorFromHEX(txt_color_t->value->int32);
  }

  Tuple *eye_color_t = dict_find(iter, MESSAGE_KEY_EyeColor);
  if(eye_color_t) {
    s_settings.EyeColor = GColorFromHEX(eye_color_t->value->int32);
  }

  Tuple *eyeg_color_t = dict_find(iter, MESSAGE_KEY_EyeGrayedColor);
  if(eyeg_color_t) {
    s_settings.EyeGrayedColor = GColorFromHEX(eyeg_color_t->value->int32);
  }

  Tuple *bat_lvl_color_t = dict_find(iter, MESSAGE_KEY_BatteryLevelColor);
  if(bat_lvl_color_t) {
    s_settings.BatteryLevelColor = GColorFromHEX(bat_lvl_color_t->value->int32);
  }

  Tuple *steps_bar_color_t = dict_find(iter, MESSAGE_KEY_StepCounterColor);
  if(steps_bar_color_t) {
    s_settings.StepCounterColor = GColorFromHEX(steps_bar_color_t->value->int32);
  }

  Tuple *dm_color_t = dict_find(iter, MESSAGE_KEY_DarkMode);
  if(dm_color_t) {
    bool darkMode = dm_color_t->value->int32 == 1;
    s_settings.BackgroundColor = darkMode ? GColorBlack : GColorWhite;
    s_settings.TextColor = darkMode ? GColorWhite : GColorBlack;
    s_settings.EyeColor = darkMode ? GColorWhite : GColorBlack;
    s_settings.EyeGrayedColor = darkMode ? GColorDarkGray : GColorLightGray;
    s_settings.BatteryLevelColor = darkMode ? GColorWhite : GColorBlack;
    s_settings.StepCounterColor = darkMode ? GColorWhite : GColorBlack;
  }

  Tuple *bat_bar_gray_t = dict_find(iter, MESSAGE_KEY_GrayBatteryBar);
  if (bat_bar_gray_t) {
    if (bat_bar_gray_t->value->int32 == 1) s_settings.BatteryLevelColor = GColorDarkGray;
  }

  Tuple *steps_bar_gray_t = dict_find(iter, MESSAGE_KEY_GrayStepCounter);
  if (steps_bar_gray_t) {
    if (steps_bar_gray_t->value->int32 == 1) s_settings.StepCounterColor = GColorDarkGray;
  }

  // XANA Eye
  Tuple *void_on_disconnect_t = dict_find(iter, MESSAGE_KEY_VoidOnDisconnect);
  if(void_on_disconnect_t) {
    s_settings.EyeOnDisconnected &= ~(EYE_MODIFIER_VOID);
    if (void_on_disconnect_t->value->int32 == 1) {
      s_settings.EyeOnDisconnected |= EYE_MODIFIER_VOID;
    }
  }

  Tuple *gray_on_disconnect_t = dict_find(iter, MESSAGE_KEY_GrayOnDisconnect);
  if(gray_on_disconnect_t) {
    s_settings.EyeOnDisconnected &= ~(EYE_MODIFIER_GRAY);
    if (gray_on_disconnect_t->value->int32 == 1) {
      s_settings.EyeOnDisconnected |= EYE_MODIFIER_GRAY;
    }
  }

  // Battery
  Tuple *bat_percent_t = dict_find(iter, MESSAGE_KEY_DisplayBatteryPercentage);
  if (bat_percent_t) {
    s_settings.DisplayBatteryPercentage = bat_percent_t->value->int32 == 1;
  }

  Tuple *bat_bar_t = dict_find(iter, MESSAGE_KEY_DisplayBatteryBar);
  if (bat_bar_t) {
    s_settings.DisplayBatteryBar = bat_bar_t->value->int32 == 1;
  }

  // Steps
  Tuple *steps_counter_t = dict_find(iter, MESSAGE_KEY_DisplayStepsCounter);
  if (steps_counter_t) {
    s_settings.DisplayStepsCounter = steps_counter_t->value->int32 == 1;
  }

  Tuple *steps_bar_t = dict_find(iter, MESSAGE_KEY_DisplayStepsBar);
  if (steps_bar_t) {
    s_settings.DisplayStepsBar = steps_bar_t->value->int32 == 1;
  }

  Tuple *steps_bar_max_t = dict_find(iter, MESSAGE_KEY_StepsBarMax);
  if (steps_bar_max_t) {
    s_settings.StepsBarMax = steps_bar_max_t->value->int32;
  }

  // Misc
  Tuple *vibrate_on_disc = dict_find(iter, MESSAGE_KEY_VibrateOnDisconnect);
  if (vibrate_on_disc) {
    s_settings.VibrateOnDisconnect = vibrate_on_disc->value->int32 == 1;
  }

  save_settings();
  
  if (s_new_settings_handler) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Handler valid, calling handler");
    s_new_settings_handler(&s_settings);
  }
}

void settings_init() {
    load_settings(s_settings);

    // Open AppMessage connection
    app_message_register_inbox_received(prv_inbox_received_handler);
    app_message_open(128, 128);
}

void settings_new_subscribe(NewSettingsHandler handler) {
    s_new_settings_handler = handler;
}

ClaySettings* settings_get() {
    return &s_settings;
}