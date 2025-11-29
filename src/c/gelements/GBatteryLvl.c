#include "GBatteryLvl.h"

static GFont s_battery_font;
static TextLayer *s_battery_text_layer;
static Layer *s_battery_layer;
static int s_battery_level;

static void prv_battery_rect_update_proc(Layer *layer, GContext *ctx) {
  ClaySettings *settings = settings_get();
  if (!settings->DisplayBatteryBar) {
    layer_set_hidden(layer, true);
    return;
  }

  layer_set_hidden(layer, false);

  GRect bounds = layer_get_bounds(layer);

  // Find the height of the bar
  int height = (s_battery_level * bounds.size.h) / 100;

  // Draw the background
  graphics_context_set_fill_color(ctx, settings->BackgroundColor);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_stroke_color(ctx, settings->TextColor);
  graphics_draw_rect(ctx, GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h));
  graphics_context_set_fill_color(ctx, settings->BatteryLevelColor);
  graphics_fill_rect(ctx, GRect(bounds.origin.x, bounds.origin.y + bounds.size.h - height, bounds.size.w, height), 0, GCornerNone);
}

void battery_lvl_init(Layer* window_layer, GRect bounds) {
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_15));
  s_battery_text_layer = text_layer_create(GRect_BatText(bounds));
  text_layer_set_font(s_battery_text_layer, s_battery_font);
  text_layer_set_text_alignment(s_battery_text_layer, GTextAlignmentRight);
  text_layer_set_text(s_battery_text_layer, "50%");
  layer_add_child(window_layer, text_layer_get_layer(s_battery_text_layer));

  s_battery_layer = layer_create(GRect_BatRect(bounds));
  layer_set_update_proc(s_battery_layer, prv_battery_rect_update_proc);
  layer_add_child(window_layer, s_battery_layer);
}

void battery_lvl_update(int battery_level) {
  static char buffer[5];
  snprintf(buffer, 5, "%d%%", battery_level);
  text_layer_set_text(s_battery_text_layer, buffer);

  s_battery_level = battery_level;
  layer_mark_dirty(s_battery_layer);
}

void battery_lvl_apply_settings(ClaySettings* settings) {
  text_layer_set_background_color(s_battery_text_layer, GColorClear);
  text_layer_set_text_color(s_battery_text_layer, settings->TextColor);
  layer_set_hidden(text_layer_get_layer(s_battery_text_layer), !settings->DisplayBatteryPercentage);

  layer_mark_dirty(s_battery_layer);
}

void battery_lvl_layout_update(GRect bounds, bool obstructed) {
    layer_set_frame(text_layer_get_layer(s_battery_text_layer), obstructed ? GRect_BatText_obstructed(bounds) : GRect_BatText(bounds));
    layer_set_frame(s_battery_layer, obstructed ? GRect_BatRect_obstructed(bounds) : GRect_BatRect(bounds));
}

void battery_lvl_deinit() {
  fonts_unload_custom_font(s_battery_font);
  text_layer_destroy(s_battery_text_layer);
  layer_destroy(s_battery_layer);
}