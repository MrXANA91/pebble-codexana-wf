#pragma once

#include <pebble.h>

#include "../config.h"
#include "../modules/settings.h"

void step_tracker_init(Layer* window_layer, GRect bounds);
void step_tracker_update(int steps);
void step_tracker_apply_settings(ClaySettings* settings);
void step_tracker_layout_update(GRect bounds, bool obstructed);
void step_tracker_deinit();