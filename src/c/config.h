#pragma once

#include <pebble.h>

// #define DEBUG
// #define FAKE_STEP_COUNTERS  3261
// #define FAKE_STEP_COUNTERS  1896
// #define FAKE_STEP_COUNTERS  5039

#define TIME_TEXT_HEIGHT  40
#define DATE_TEXT_HEIGHT  40
#define BATT_TEXT_HEIGHT  20
#define BATT_RECT_HEIGHT  PBL_IF_ROUND_ELSE(60,70)
#define STEP_TEXT_HEIGHT  BATT_TEXT_HEIGHT
#define STEP_RECT_HEIGHT  BATT_RECT_HEIGHT

#define GRect_Date(bounds) GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, DATE_TEXT_HEIGHT)

#define GRect_XANA_default(bounds) GRect(bounds.origin.x + 1, bounds.origin.y, bounds.size.w - 1, bounds.size.h)
#define GRect_XANA_obstructed(bounds) GRect(bounds.origin.x + 1, bounds.origin.y, bounds.size.w - 1, gbitmap_get_bounds(s_xana_bitmap).size.h)

#define GRect_DHours(bounds) GRect(bounds.origin.x, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Hours(bounds) GRect(bounds.origin.x + ((bounds.size.w / 2) - 5) / 2, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Colon(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) - 5, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, 10, TIME_TEXT_HEIGHT)
#define GRect_DMinutes(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) + 5, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)
#define GRect_Minutes(bounds) GRect(bounds.origin.x + (bounds.size.w / 2) + 5 + ((bounds.size.w / 2) - 5) / 2, bounds.origin.y + bounds.size.h - TIME_TEXT_HEIGHT, ((bounds.size.w / 2) - 5) / 2, TIME_TEXT_HEIGHT)

#define GRect_BatText(bounds) GRect(bounds.origin.x + (bounds.size.w / 2), bounds.origin.y + DATE_TEXT_HEIGHT - PBL_IF_ROUND_ELSE(15,5), bounds.size.w / 2, BATT_TEXT_HEIGHT)
#define GRect_BatText_obstructed(bounds) GRect(bounds.origin.x + (bounds.size.w / 2), bounds.origin.y, bounds.size.w / 2, BATT_TEXT_HEIGHT)
#define GRect_BatRect(bounds) GRect(bounds.origin.x + (bounds.size.w * 3 / 4), bounds.origin.y + DATE_TEXT_HEIGHT - PBL_IF_ROUND_ELSE(15,5) + BATT_TEXT_HEIGHT, bounds.size.w / 4, BATT_RECT_HEIGHT)
#define GRect_BatRect_obstructed(bounds) GRect(bounds.origin.x + (bounds.size.w * 3 / 4), bounds.origin.y + BATT_TEXT_HEIGHT, bounds.size.w / 4, BATT_RECT_HEIGHT - 7)

#define GRect_StepText(bounds) GRect(bounds.origin.x, bounds.origin.y + DATE_TEXT_HEIGHT - PBL_IF_ROUND_ELSE(15,5), bounds.size.w / 2, STEP_TEXT_HEIGHT)
#define GRect_StepText_obstructed(bounds) GRect(bounds.origin.x, bounds.origin.y, bounds.size.w / 2, STEP_TEXT_HEIGHT)
#define GRect_StepRect(bounds) GRect(bounds.origin.x, bounds.origin.y + DATE_TEXT_HEIGHT - PBL_IF_ROUND_ELSE(15,5) + STEP_TEXT_HEIGHT, bounds.size.w / 4, STEP_RECT_HEIGHT)
#define GRect_StepRect_obstructed(bounds) GRect(bounds.origin.x, bounds.origin.y + STEP_TEXT_HEIGHT, bounds.size.w / 4, STEP_RECT_HEIGHT - 7)
