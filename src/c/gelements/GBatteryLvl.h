#pragma once

#include <pebble.h>

#include "../config.h"
#include "../modules/settings.h"

void battery_lvl_init(Layer* window_layer, GRect bounds);
void battery_lvl_update(int battery_level);
void battery_lvl_apply_settings(ClaySettings* settings);
void battery_lvl_layout_update(GRect bounds, bool obstructed);
void battery_lvl_deinit();