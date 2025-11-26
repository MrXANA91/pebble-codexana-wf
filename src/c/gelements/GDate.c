#include "GDate.h"

static GFont s_date_font;
static TextLayer *s_date_layer;

void date_init(Layer* window_layer, GRect bounds) {
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_26));
  s_date_layer = text_layer_create(GRect_Date(bounds));
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "Unknown");
  // layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

void date_update(struct tm * current_time) {
  static char s_date_buffer[12];

  strftime(s_date_buffer, sizeof(s_date_buffer), 
    #ifdef DEBUG
    "WED 30"
    #else
    "%a %d"
    #endif
    , current_time);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

void date_apply_settings(ClaySettings* settings) {
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings->TextColor);
}

void date_layout_update(GRect bounds, bool obstructed) {
  (void)bounds;
  layer_set_hidden(text_layer_get_layer(s_date_layer), obstructed);
}

void date_deinit() {
  fonts_unload_custom_font(s_date_font);
  text_layer_destroy(s_date_layer);
}