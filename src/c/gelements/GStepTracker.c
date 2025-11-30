#include "GStepTracker.h"

static GFont s_step_font;
static TextLayer *s_step_text_layer;
static Layer *s_step_layer;
static int s_steps;

static void prv_step_rect_update_proc(Layer *layer, GContext *ctx) {
    ClaySettings *settings = settings_get();
  if (!settings->DisplayStepsBar) {
    layer_set_hidden(layer, true);
    return;
  }

  layer_set_hidden(layer, false);

  GRect bounds = layer_get_bounds(layer);

  // Find the height of the bar
  int height = s_steps > settings->StepsBarMax ? bounds.size.h : (s_steps * bounds.size.h) / settings->StepsBarMax;

  // Draw the background
  graphics_context_set_fill_color(ctx, settings->BackgroundColor);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_stroke_color(ctx, settings->TextColor);
  graphics_draw_rect(ctx, GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h));
  graphics_context_set_fill_color(ctx, settings->StepCounterColor);
  graphics_fill_rect(ctx, GRect(bounds.origin.x, bounds.origin.y + bounds.size.h - height, bounds.size.w, height), 0, GCornerNone);
}

void step_tracker_init(Layer* window_layer, GRect bounds) {
    s_step_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GUNSHIP_15));
    s_step_text_layer = text_layer_create(GRect_StepText(bounds));
    text_layer_set_font(s_step_text_layer, s_step_font);
    text_layer_set_text_alignment(s_step_text_layer, GTextAlignmentLeft);
    text_layer_set_text(s_step_text_layer, "---");
    layer_add_child(window_layer, text_layer_get_layer(s_step_text_layer));

    s_step_layer = layer_create(GRect_StepRect(bounds));
    layer_set_update_proc(s_step_layer, prv_step_rect_update_proc);
    layer_add_child(window_layer, s_step_layer);
}

void step_tracker_update(int steps) {
    static char buffer[8];
    snprintf(buffer, 8, "%d", steps);
    text_layer_set_text(s_step_text_layer, buffer);

    s_steps = steps;
    layer_mark_dirty(s_step_layer);
}

void step_tracker_apply_settings(ClaySettings* settings) {
    text_layer_set_background_color(s_step_text_layer, GColorClear);
    text_layer_set_text_color(s_step_text_layer, settings->TextColor);
    layer_set_hidden(text_layer_get_layer(s_step_text_layer), !settings->DisplayStepsCounter);

    layer_mark_dirty(s_step_layer);
}

void step_tracker_layout_update(GRect bounds, bool obstructed) {
    layer_set_frame(text_layer_get_layer(s_step_text_layer), obstructed ? GRect_StepText_obstructed(bounds) : GRect_StepText(bounds));
    layer_set_frame(s_step_layer, obstructed ? GRect_StepRect_obstructed(bounds) : GRect_StepRect(bounds));
}

void step_tracker_deinit() {
    fonts_unload_custom_font(s_step_font);
    text_layer_destroy(s_step_text_layer);
    layer_destroy(s_step_layer);
}