#pragma once

#include <pebble.h>

#include "../config.h"
#include "../modules/settings.h"

void date_init(Layer* window_layer, GRect bounds);
void date_update(struct tm * current_time);
void date_apply_settings(ClaySettings* settings);
void date_layout_update(GRect bounds, bool obstructed);
void date_deinit();