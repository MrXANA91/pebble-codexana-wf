#include "settings.h"

#include <pebble.h>

// #define DEBUG

#define TIME_TEXT_HEIGHT  PBL_IF_ROUND_ELSE(40, 45)
#define DATE_TEXT_HEIGHT  40

#define GRect_Date(bounds) GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, DATE_TEXT_HEIGHT)
#define GRect_XANA_default(bounds) GRect(bounds.origin.x + 1, bounds.origin.y, bounds.size.w - 1, bounds.size.h)
#define GRect_XANA_obstructed(bounds) GRect(bounds.origin.x + 1, bounds.origin.y, bounds.size.w - 1, gbitmap_get_bounds(s_xana_bitmap).size.h)
#define GRect_DHours(bounds) GRect(bounds.origin.x, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Hours(bounds) GRect(bounds.origin.x + ((bounds.size.w / 2) - 5) / 2, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Colon(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) - 5, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, 10, TIME_TEXT_HEIGHT)
#define GRect_DMinutes(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) + 5, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Minutes(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) + 5 + ((bounds.size.w / 2) - 5) / 2, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)

static Window *s_window;

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_bitmap;
static GBitmap *s_xana_void_bitmap;
static GColor *s_xana_color;
static GColor *s_xana_void_color;

static GFont s_time_font;
static TextLayer *s_time_dhours_layer;
static TextLayer *s_time_hours_layer;
static TextLayer *s_time_colon_layer;
static TextLayer *s_time_dminutes_layer;
static TextLayer *s_time_minutes_layer;

static GFont s_date_font;
static TextLayer *s_date_layer;

static ClaySettings settings;

static void prv_update_display();

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Colors
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  Tuple *txt_color_t = dict_find(iter, MESSAGE_KEY_TextColor);
  if(txt_color_t) {
    settings.TextColor = GColorFromHEX(txt_color_t->value->int32);
  }

  Tuple *eye_color_t = dict_find(iter, MESSAGE_KEY_EyeColor);
  if(eye_color_t) {
    settings.EyeColor = GColorFromHEX(eye_color_t->value->int32);
  }

  Tuple *eyeg_color_t = dict_find(iter, MESSAGE_KEY_EyeGrayedColor);
  if(eyeg_color_t) {
    settings.EyeGrayedColor = GColorFromHEX(eyeg_color_t->value->int32);
  }

  Tuple *dm_color_t = dict_find(iter, MESSAGE_KEY_DarkMode);
  if(dm_color_t) {
    bool darkMode = dm_color_t->value->int32 == 1;
    settings.BackgroundColor = darkMode ? GColorBlack : GColorWhite;
    settings.TextColor = darkMode ? GColorWhite : GColorBlack;
    settings.EyeColor = darkMode ? GColorWhite : GColorBlack;
    settings.EyeGrayedColor = darkMode ? GColorDarkGray : GColorLightGray;
  }

  // XANA Eye
  Tuple *void_on_disconnect_t = dict_find(iter, MESSAGE_KEY_VoidOnDisconnect);
  if(void_on_disconnect_t) {
    settings.EyeOnDisconnected &= ~(EYE_MODIFIER_VOID);
    if (void_on_disconnect_t->value->int32 == 1) {
      settings.EyeOnDisconnected |= EYE_MODIFIER_VOID;
    }
  }

  Tuple *gray_on_disconnect_t = dict_find(iter, MESSAGE_KEY_GrayOnDisconnect);
  if(gray_on_disconnect_t) {
    settings.EyeOnDisconnected &= ~(EYE_MODIFIER_GRAY);
    if (gray_on_disconnect_t->value->int32 == 1) {
      settings.EyeOnDisconnected |= EYE_MODIFIER_GRAY;
    }
  }

  // Misc
  Tuple *vibrate_on_disc = dict_find(iter, MESSAGE_KEY_VibrateOnDisconnect);
  if (vibrate_on_disc) {
    settings.VibrateOnDisconnect = vibrate_on_disc->value->int32 == 1;
  }

  save_settings(&settings);
  prv_update_display();
}

static struct tm *current_time;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  current_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  // Display this time on the TextLayer
  static char s_hours_buffer[3];
  static char s_dhours_buffer[3];
  static char s_minutes_buffer[3];
  static char s_dminutes_buffer[3];
  static char s_date_buffer[12];

  #ifdef DEBUG
  static int counter = 0;
  counter++;
  snprintf(s_dhours_buffer, sizeof(s_dhours_buffer), "%02d", counter % 24);
  snprintf(s_dminutes_buffer, sizeof(s_dminutes_buffer), "%02d", counter % 60);
  #else
  strftime(s_dhours_buffer, sizeof(s_dhours_buffer), clock_is_24h_style() ? "%H" : "%I", current_time);
  strftime(s_dminutes_buffer, sizeof(s_dminutes_buffer), "%M", current_time);
  #endif

  s_hours_buffer[0] = s_dhours_buffer[1];
  s_hours_buffer[1] = 0x00;
  s_dhours_buffer[1] = 0x00;
  s_minutes_buffer[0] = s_dminutes_buffer[1];
  s_minutes_buffer[1] = 0x00;
  s_dminutes_buffer[1] = 0x00;

  text_layer_set_text(s_time_dhours_layer, s_dhours_buffer);
  text_layer_set_text(s_time_hours_layer, s_hours_buffer);
  text_layer_set_text(s_time_dminutes_layer, s_dminutes_buffer);
  text_layer_set_text(s_time_minutes_layer, s_minutes_buffer);

  strftime(s_date_buffer, sizeof(s_date_buffer), 
    #ifdef DEBUG
    "WED 30"
    #else
    "%a %d"
    #endif
    , current_time);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void prv_update_xana_eye(EyeModifiersEnum_t modifiers) {
  if (modifiers == EYE_MODIFIER_NONE) {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
    *s_xana_color = settings.EyeColor;
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), false);
    return;
  }

  if ((modifiers & EYE_MODIFIER_HIDE) != 0) {
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), true);
  } else {
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), false);
  }

  if ((modifiers & EYE_MODIFIER_VOID) != 0) {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_void_bitmap);
  } else {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  }

  if ((modifiers & EYE_MODIFIER_GRAY) != 0) {
    if (s_xana_color) *s_xana_color = settings.EyeGrayedColor;
    if (s_xana_void_color) *s_xana_void_color = settings.EyeGrayedColor;
  } else {
    if (s_xana_color) *s_xana_color = settings.EyeColor;
    if (s_xana_void_color) *s_xana_void_color = settings.EyeColor;
  }
}

static void bluetooth_callback(bool connected) {
  prv_update_xana_eye(connected ? settings.EyeOnConnected : settings.EyeOnDisconnected);

  if (settings.VibrateOnDisconnect && !connected) {
    vibes_double_pulse();
  }
}

/// @brief Get the pointer to the black color in the bitmap palette
/// @param palette 
/// @param bitmap 
/// @attention Original code from [EurophoricPenguin/Fiftyeight](https://github.com/EuphoricPenguin/Fiftyeight/blob/main/src/c/fiftyeight.c)
static void prv_get_black_palette(GColor **palette_ptr_ptr, GBitmap* bitmap) {
  if (!palette_ptr_ptr) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No dest palette");
    return;
  }
  if (!bitmap) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No bitmap");
    return;
  }
  GColor *palette_array = gbitmap_get_palette(bitmap);
  if (!palette_array) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Bitmap has no palette");
    return;
  }
  // Get the bitmap format to determine palette size
  GBitmapFormat format = gbitmap_get_format(bitmap);
  int palette_size = 0;
  switch (format)
  {
    case GBitmapFormat1BitPalette:
      palette_size = 2;
      break;
    case GBitmapFormat2BitPalette:
      palette_size = 4;
      break;
    case GBitmapFormat4BitPalette:
      palette_size = 16;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Not a palette-based format, can't invert");
      return;
  }
  // Retrieving 
  for (int i = 0; i < palette_size; i++)
  {
    if (gcolor_equal(palette_array[i], GColorBlack))
    {
      *palette_ptr_ptr = &palette_array[i];
      return;
    }
  }
  APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to set palette");
  return;
}

static void prv_update_display() {
  window_set_background_color(s_window, settings.BackgroundColor);
  bitmap_layer_set_background_color(s_xana_layer, settings.BackgroundColor);
  prv_update_xana_eye(settings.EyeOnConnected);

  text_layer_set_background_color(s_time_hours_layer, GColorClear);
  text_layer_set_text_color(s_time_hours_layer, settings.TextColor);
  text_layer_set_background_color(s_time_dhours_layer, GColorClear);
  text_layer_set_text_color(s_time_dhours_layer, settings.TextColor);
  text_layer_set_background_color(s_time_colon_layer, GColorClear);
  text_layer_set_text_color(s_time_colon_layer, settings.TextColor);
  text_layer_set_background_color(s_time_minutes_layer, GColorClear);
  text_layer_set_text_color(s_time_minutes_layer, settings.TextColor);
  text_layer_set_background_color(s_time_dminutes_layer, GColorClear);
  text_layer_set_text_color(s_time_dminutes_layer, settings.TextColor);

  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);

  // XANA logo
  s_xana_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA);
  s_xana_void_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_VOID);
  prv_get_black_palette(&s_xana_color, s_xana_bitmap);
  prv_get_black_palette(&s_xana_void_color, s_xana_void_bitmap);
  s_xana_layer = bitmap_layer_create(GRect_XANA_default(bounds));
  bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  bitmap_layer_set_compositing_mode(s_xana_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_xana_layer));

  #ifdef PBL_ROUND
  bounds.origin.x += 20;
  bounds.origin.y += 20;
  bounds.size.w -= 40;
  bounds.size.h -= 45;
  #endif

  // Time
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_33));
  s_time_dhours_layer = text_layer_create(GRect_DHours(bounds));
  text_layer_set_font(s_time_dhours_layer, s_time_font);
  text_layer_set_text_alignment(s_time_dhours_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_dhours_layer, "0");
  layer_add_child(window_layer, text_layer_get_layer(s_time_dhours_layer));
  s_time_hours_layer = text_layer_create(GRect_Hours(bounds));
  text_layer_set_font(s_time_hours_layer, s_time_font);
  text_layer_set_text_alignment(s_time_hours_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_hours_layer, "0");
  layer_add_child(window_layer, text_layer_get_layer(s_time_hours_layer));

  s_time_dminutes_layer = text_layer_create(GRect_DMinutes(bounds));
  text_layer_set_font(s_time_dminutes_layer, s_time_font);
  text_layer_set_text_alignment(s_time_dminutes_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_dminutes_layer, "0");
  layer_add_child(window_layer, text_layer_get_layer(s_time_dminutes_layer));
  s_time_minutes_layer = text_layer_create(GRect_Minutes(bounds));
  text_layer_set_font(s_time_minutes_layer, s_time_font);
  text_layer_set_text_alignment(s_time_minutes_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_minutes_layer, "0");
  layer_add_child(window_layer, text_layer_get_layer(s_time_minutes_layer));

  s_time_colon_layer = text_layer_create(GRect_Colon(bounds));
  text_layer_set_font(s_time_colon_layer, s_time_font);
  text_layer_set_text_alignment(s_time_colon_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_colon_layer, ":");
  layer_add_child(window_layer, text_layer_get_layer(s_time_colon_layer));

  // Date
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_26));
  s_date_layer = text_layer_create(GRect_Date(bounds));
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "Unknown");
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  prv_update_display();
}

static void prv_window_unload(Window *window) {
  gbitmap_destroy(s_xana_bitmap);
  gbitmap_destroy(s_xana_void_bitmap);
  bitmap_layer_destroy(s_xana_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  text_layer_destroy(s_time_dhours_layer);
  text_layer_destroy(s_time_hours_layer);
  text_layer_destroy(s_time_colon_layer);
  text_layer_destroy(s_time_dminutes_layer);
  text_layer_destroy(s_time_minutes_layer);
  text_layer_destroy(s_date_layer);
}

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  GRect full_bounds = layer_get_bounds(window_get_root_layer(s_window));
  if (!grect_equal(&full_bounds, &final_unobstructed_screen_area)) {
    // Screen is about to become obstructed, hide the date
    layer_set_hidden(text_layer_get_layer(s_date_layer), true);
    // Move the eye
    layer_set_frame(bitmap_layer_get_layer(s_xana_layer), GRect_XANA_obstructed(final_unobstructed_screen_area));
    // Move the time
    layer_set_frame(text_layer_get_layer(s_time_dhours_layer), GRect_DHours(final_unobstructed_screen_area));
    layer_set_frame(text_layer_get_layer(s_time_hours_layer), GRect_Hours(final_unobstructed_screen_area));
    layer_set_frame(text_layer_get_layer(s_time_colon_layer), GRect_Colon(final_unobstructed_screen_area));
    layer_set_frame(text_layer_get_layer(s_time_dminutes_layer), GRect_DMinutes(final_unobstructed_screen_area));
    layer_set_frame(text_layer_get_layer(s_time_minutes_layer), GRect_Minutes(final_unobstructed_screen_area));
  }
}

static void prv_unobstructed_did_change(void *context) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect full_bounds = layer_get_bounds(window_layer);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  if (grect_equal(&full_bounds, &bounds)) {
    // Screen is no longer obstructed, show the date
    layer_set_hidden(text_layer_get_layer(s_date_layer), false);
    // Move the eye
    layer_set_frame(bitmap_layer_get_layer(s_xana_layer), GRect_XANA_default(bounds));
    // Move the time
    layer_set_frame(text_layer_get_layer(s_time_dhours_layer), GRect_DHours(bounds));
    layer_set_frame(text_layer_get_layer(s_time_hours_layer), GRect_Hours(bounds));
    layer_set_frame(text_layer_get_layer(s_time_colon_layer), GRect_Colon(bounds));
    layer_set_frame(text_layer_get_layer(s_time_dminutes_layer), GRect_DMinutes(bounds));
    layer_set_frame(text_layer_get_layer(s_time_minutes_layer), GRect_Minutes(bounds));
  }
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

  tick_timer_service_subscribe(
  #ifdef DEBUG
  SECOND_UNIT
  #else
  MINUTE_UNIT
  #endif
  , tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  UnobstructedAreaHandlers handlers = {
    .will_change = prv_unobstructed_will_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe(handlers, NULL);

  update_time();

  bluetooth_callback(connection_service_peek_pebble_app_connection());
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  load_settings(&settings);

  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
