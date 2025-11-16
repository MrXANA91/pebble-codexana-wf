#include "settings.h"

#include <pebble.h>

static Window *s_window;

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_white_bitmap;
static GBitmap *s_xana_black_bitmap;

static GFont s_dig_time_font;
static TextLayer *s_dig_time_layer;
static GFont s_dig_date_font;
static TextLayer *s_dig_date_layer;

static ClaySettings settings;

static void prv_update_display();

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Read color preferences
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
  if(fg_color_t) {
    settings.ForegroundColor = GColorFromHEX(fg_color_t->value->int32);
  }

  // Read boolean preferences
  Tuple *show_logo_t = dict_find(iter, MESSAGE_KEY_ShowLogo);
  if(show_logo_t) {
    settings.ShowLogo = show_logo_t->value->int32 == 1;
  }

  Tuple *hide_logo_on_disc = dict_find(iter, MESSAGE_KEY_HideLogoOnDisconnect);
  if(hide_logo_on_disc) {
    settings.HideLogoOnDisconnect = hide_logo_on_disc->value->int32 == 1;
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
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", current_time);
  text_layer_set_text(s_dig_time_layer, s_buffer);

  static char s_date_buffer[12];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%F", current_time);
  text_layer_set_text(s_dig_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void bluetooth_callback(bool connected) {
  if (settings.ShowLogo) {
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), !connected);
  }
}

static void prv_update_display() {
  window_set_background_color(s_window, settings.BackgroundColor);

  if (settings.BackgroundColor.argb == GColorBlack.argb) {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_black_bitmap);
  } else {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_white_bitmap);
  }
  layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), !settings.ShowLogo || (settings.BackgroundColor.argb == settings.ForegroundColor.argb));

  text_layer_set_background_color(s_dig_time_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_time_layer, settings.ForegroundColor);

  text_layer_set_background_color(s_dig_date_layer, settings.BackgroundColor);
  text_layer_set_text_color(s_dig_date_layer, settings.ForegroundColor);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // XANA logo
  s_xana_black_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_BLACK);
  s_xana_white_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_WHITE);
  s_xana_layer = bitmap_layer_create(bounds);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_xana_layer));

  // Digital text
  s_dig_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_30));
  s_dig_time_layer = text_layer_create(
    GRect(0, 124, bounds.size.w, 40)
  );
  text_layer_set_font(s_dig_time_layer, s_dig_time_font);
  text_layer_set_text_alignment(s_dig_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_time_layer, "00:00");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_time_layer));

  s_dig_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_14));
  s_dig_date_layer = text_layer_create(
    GRect(0, 10, bounds.size.w, 20)
  );
  text_layer_set_font(s_dig_date_layer, s_dig_date_font);
  text_layer_set_text_alignment(s_dig_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_date_layer, "Unknown");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_date_layer));

  prv_update_display();
}

static void prv_window_unload(Window *window) {
  gbitmap_destroy(s_xana_white_bitmap);
  gbitmap_destroy(s_xana_black_bitmap);
  bitmap_layer_destroy(s_xana_layer);
  fonts_unload_custom_font(s_dig_time_font);
  text_layer_destroy(s_dig_time_layer);
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
