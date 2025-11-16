#include <pebble.h>

static Window *s_window;

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_bitmap;

static GFont s_dig_time_font;
static TextLayer *s_dig_time_layer;
static GFont s_dig_date_font;
static TextLayer *s_dig_date_layer;

static struct tm *current_time;

// static TextLayer *s_analog_12_layer;
// static TextLayer *s_analog_1_layer;
// static TextLayer *s_analog_2_layer;
// static TextLayer *s_analog_3_layer;
// static TextLayer *s_analog_4_layer;
// static TextLayer *s_analog_5_layer;
// static TextLayer *s_analog_6_layer;
// static TextLayer *s_analog_7_layer;
// static TextLayer *s_analog_8_layer;
// static TextLayer *s_analog_9_layer;
// static TextLayer *s_analog_10_layer;
// static TextLayer *s_analog_11_layer;

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

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GColor bg = GColorWhite;
  GColor fg = GColorBlack;

  window_set_background_color(window, bg);

  // XANA logo
  s_xana_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_WHITE);
  s_xana_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_xana_layer));

  // Digital text
  s_dig_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_30));
  s_dig_time_layer = text_layer_create(
    GRect(0, 124, bounds.size.w, 40)
  );
  text_layer_set_background_color(s_dig_time_layer, bg);
  text_layer_set_text_color(s_dig_time_layer, fg);
  text_layer_set_font(s_dig_time_layer, s_dig_time_font);
  text_layer_set_text_alignment(s_dig_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_time_layer, "00:00");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_time_layer));

  s_dig_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_14));
  s_dig_date_layer = text_layer_create(
    GRect(0, 10, bounds.size.w, 20)
  );
  text_layer_set_background_color(s_dig_date_layer, bg);
  text_layer_set_text_color(s_dig_date_layer, fg);
  text_layer_set_font(s_dig_date_layer, s_dig_date_font);
  text_layer_set_text_alignment(s_dig_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dig_date_layer, "Unknown");
  layer_add_child(window_layer, text_layer_get_layer(s_dig_date_layer));
}

static void prv_window_unload(Window *window) {
  gbitmap_destroy(s_xana_bitmap);
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

  update_time();
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
