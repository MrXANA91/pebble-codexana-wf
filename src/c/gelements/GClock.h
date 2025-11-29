#pragma once

#include <pebble.h>

#include "../config.h"
#include "../modules/settings.h"

void clock_init(Layer* window_layer, GRect bounds);
void clock_update(struct tm * current_time);
void clock_apply_settings(ClaySettings* settings);
void clock_layout_update(GRect bounds, bool obstructed);
void clock_deinit();