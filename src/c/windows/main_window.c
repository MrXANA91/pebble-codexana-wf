#include "main_window.h"

static Window *s_window;

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context);
static void prv_apply_settings(ClaySettings *settings);

static void prv_adjust_fullbounds_for_round(GRect *bounds) {
  #ifdef PBL_ROUND
  GRect fullbounds = layer_get_bounds(window_get_root_layer(s_window));
  if (grect_equal(&fullbounds, bounds)) {
    bounds->origin.x += 20;
    bounds->origin.y += 20;
    bounds->size.w -= 40;
    bounds->size.h -= 45;
  }
  #endif
}

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  GRect full_bounds = layer_get_bounds(window_get_root_layer(s_window));
  if (!grect_equal(&full_bounds, &final_unobstructed_screen_area)) {
    // Screen is about to become obstructed, hide the date
    date_layout_update(final_unobstructed_screen_area, true);
    // Move the eye
    eye_layout_update(final_unobstructed_screen_area, true);
    // Move the time
    clock_layout_update(final_unobstructed_screen_area, true);
    // Move the battery text & rectangle
    battery_lvl_layout_update(final_unobstructed_screen_area, true);
  }
}

static void prv_unobstructed_did_change(void *context) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect full_bounds = layer_get_bounds(window_layer);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  if (grect_equal(&full_bounds, &bounds)) {
    // Screen is no longer obstructed, show the date
    date_layout_update(bounds, false);
    // Move the eye
    eye_layout_update(bounds, false);
    // Move the time
    clock_layout_update(bounds, false);
    // Move the battery text & rectangle
    battery_lvl_layout_update(bounds, false);
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);

  eye_init(window_layer, bounds);

  // -- Adjusting bounds specifically for Round displays --
  prv_adjust_fullbounds_for_round(&bounds);

  clock_init(window_layer, bounds);
  date_init(window_layer, bounds);
  battery_lvl_init(window_layer, bounds);

  prv_apply_settings(settings_get());
  prv_unobstructed_will_change(bounds, NULL);
}

static void prv_window_unload(Window *window) {
  date_deinit();
  clock_deinit();
  eye_deinit();
  battery_lvl_deinit();
}

static void prv_apply_settings(ClaySettings *settings) {
    window_set_background_color(s_window, settings->BackgroundColor);
    
    // XANA Eye
    eye_apply_settings(settings);

    // Time and date
    clock_apply_settings(settings);
    date_apply_settings(settings);

    // Battery
    battery_lvl_apply_settings(settings);
}

void main_window_push() {
    if (s_window) window_destroy(s_window);

    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = prv_window_load,
        .unload = prv_window_unload,
    });
    const bool animated = true;
    window_stack_push(s_window, animated);

    settings_new_subscribe(prv_apply_settings);

    UnobstructedAreaHandlers unobstructedAreaHandlers = {
        .will_change = prv_unobstructed_will_change,
        .did_change = prv_unobstructed_did_change
    };
    unobstructed_area_service_subscribe(unobstructedAreaHandlers, NULL);
}

void main_window_destroy() {
    window_destroy(s_window);
}