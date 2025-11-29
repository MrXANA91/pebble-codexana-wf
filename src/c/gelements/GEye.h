#pragma once

#include <pebble.h>

#include "../config.h"
#include "../modules/settings.h"

void eye_init(Layer* window_layer, GRect bounds);
void eye_update(bool connected);
void eye_apply_settings(ClaySettings* settings);
void eye_layout_update(GRect bounds, bool obstructed);
void eye_deinit();