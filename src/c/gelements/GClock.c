#include "GClock.h"

static GFont s_time_font;
static TextLayer *s_time_dhours_layer;
static TextLayer *s_time_hours_layer;
static TextLayer *s_time_colon_layer;
static TextLayer *s_time_dminutes_layer;
static TextLayer *s_time_minutes_layer;

void clock_init(Layer* window_layer, GRect bounds) {
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
}

void clock_update(struct tm * current_time) {
  static char s_hours_buffer[3];
  static char s_dhours_buffer[3];
  static char s_minutes_buffer[3];
  static char s_dminutes_buffer[3];

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
}

void clock_apply_settings(ClaySettings* settings) {
  text_layer_set_background_color(s_time_hours_layer, GColorClear);
  text_layer_set_text_color(s_time_hours_layer, settings->TextColor);
  text_layer_set_background_color(s_time_dhours_layer, GColorClear);
  text_layer_set_text_color(s_time_dhours_layer, settings->TextColor);
  text_layer_set_background_color(s_time_colon_layer, GColorClear);
  text_layer_set_text_color(s_time_colon_layer, settings->TextColor);
  text_layer_set_background_color(s_time_minutes_layer, GColorClear);
  text_layer_set_text_color(s_time_minutes_layer, settings->TextColor);
  text_layer_set_background_color(s_time_dminutes_layer, GColorClear);
  text_layer_set_text_color(s_time_dminutes_layer, settings->TextColor);
}

void clock_layout_update(GRect bounds, bool obstructed) {
    (void)obstructed;
    layer_set_frame(text_layer_get_layer(s_time_dhours_layer), GRect_DHours(bounds));
    layer_set_frame(text_layer_get_layer(s_time_hours_layer), GRect_Hours(bounds));
    layer_set_frame(text_layer_get_layer(s_time_colon_layer), GRect_Colon(bounds));
    layer_set_frame(text_layer_get_layer(s_time_dminutes_layer), GRect_DMinutes(bounds));
    layer_set_frame(text_layer_get_layer(s_time_minutes_layer), GRect_Minutes(bounds));
}

void clock_deinit() {
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_time_dhours_layer);
  text_layer_destroy(s_time_hours_layer);
  text_layer_destroy(s_time_colon_layer);
  text_layer_destroy(s_time_dminutes_layer);
  text_layer_destroy(s_time_minutes_layer);
}