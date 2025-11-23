#include "settings.h"

#include <pebble.h>

static Window *s_window;

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_bitmap;
static GBitmap *s_xana_void_bitmap;
static GColor *s_xana_color;
static GColor *s_xana_void_color;

static GFont s_dig_time_font;
static TextLayer *s_dig_time_hours_layer;
static TextLayer *s_dig_time_colon_layer;
static TextLayer *s_dig_time_minutes_layer;

static GFont s_dig_date_font;
static TextLayer *s_dig_date_layer;

static ClaySettings settings;

static void prv_update_display();

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Colors
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
  if(fg_color_t) {
    settings.ForegroundColor = GColorFromHEX(fg_color_t->value->int32);
  }

  // Logo
  Tuple *show_logo_t = dict_find(iter, MESSAGE_KEY_ShowLogo);
  if(show_logo_t) {
    settings.ShowLogo = show_logo_t->value->int32 == 1;
  }

  Tuple *hide_logo_on_disc = dict_find(iter, MESSAGE_KEY_InvertLogoStateOnDisconnect);
  if(hide_logo_on_disc) {
    settings.InvertLogoStateOnDisconnect = hide_logo_on_disc->value->int32 == 1;
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
  static char s_hours_buffer[6];
  static char s_minutes_buffer[6];
  static char s_date_buffer[12];

  strftime(s_hours_buffer, sizeof(s_hours_buffer), clock_is_24h_style() ? "%H" : "%I", current_time);
  text_layer_set_text(s_dig_time_hours_layer, s_hours_buffer);
  strftime(s_minutes_buffer, sizeof(s_minutes_buffer), "%M", current_time);
  text_layer_set_text(s_dig_time_minutes_layer, s_minutes_buffer);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d", current_time);
  text_layer_set_text(s_dig_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void bluetooth_callback(bool connected) {
  if (settings.InvertLogoStateOnDisconnect) {
    bool hide = connected ? !settings.ShowLogo : settings.ShowLogo;

    // layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), hide);
    if (hide) {
      bitmap_layer_set_bitmap(s_xana_layer, s_xana_void_bitmap);
    } else {
      bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
    }
  }

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
  if (s_xana_color) {
    *s_xana_color = settings.ForegroundColor;
  }
  if (s_xana_void_color) {
    *s_xana_void_color = settings.ForegroundColor;
  }
  layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), !settings.ShowLogo);

  text_layer_set_background_color(s_dig_time_hours_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_time_hours_layer, settings.ForegroundColor);
  text_layer_set_background_color(s_dig_time_colon_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_time_colon_layer, settings.ForegroundColor);
  text_layer_set_background_color(s_dig_time_minutes_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_time_minutes_layer, settings.ForegroundColor);

  text_layer_set_background_color(s_dig_date_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_date_layer, settings.ForegroundColor);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // XANA logo
  s_xana_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA);
  s_xana_void_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_VOID);
  prv_get_black_palette(&s_xana_color, s_xana_bitmap);
  prv_get_black_palette(&s_xana_void_color, s_xana_void_bitmap);
  s_xana_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  bitmap_layer_set_compositing_mode(s_xana_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_xana_layer));

  // Digital text
  s_dig_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_33));
  s_dig_time_hours_layer = text_layer_create(
    GRect(0, 124, (bounds.size.w / 2) - 5, 40)
  );
  text_layer_set_font(s_dig_time_hours_layer, s_dig_time_font);
  text_layer_set_text_alignment(s_dig_time_hours_layer, GTextAlignmentRight);
  text_layer_set_text(s_dig_time_hours_layer, "00");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_time_hours_layer));

  s_dig_time_minutes_layer = text_layer_create(
    GRect((bounds.size.w / 2) + 5, 124, (bounds.size.w / 2) - 5, 40)
  );
  text_layer_set_font(s_dig_time_minutes_layer, s_dig_time_font);
  text_layer_set_text_alignment(s_dig_time_minutes_layer, GTextAlignmentLeft);
  text_layer_set_text(s_dig_time_minutes_layer, "00");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_time_minutes_layer));

  s_dig_time_colon_layer = text_layer_create(
    GRect((bounds.size.w / 2) - 5, 124, 10, 40)
  );
  text_layer_set_font(s_dig_time_colon_layer, s_dig_time_font);
  text_layer_set_text_alignment(s_dig_time_colon_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_time_colon_layer, ":");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_time_colon_layer));

  s_dig_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_26));
  s_dig_date_layer = text_layer_create(
    GRect(0, 0, bounds.size.w, 40)
  );
  text_layer_set_font(s_dig_date_layer, s_dig_date_font);
  text_layer_set_text_alignment(s_dig_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_date_layer, "Unknown");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_date_layer));

  prv_update_display();
}

static void prv_window_unload(Window *window) {
  gbitmap_destroy(s_xana_bitmap);
  gbitmap_destroy(s_xana_void_bitmap);
  bitmap_layer_destroy(s_xana_layer);
  fonts_unload_custom_font(s_dig_time_font);
  fonts_unload_custom_font(s_dig_date_font);
  text_layer_destroy(s_dig_time_hours_layer);
  text_layer_destroy(s_dig_time_colon_layer);
  text_layer_destroy(s_dig_time_minutes_layer);
  text_layer_destroy(s_dig_date_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

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
