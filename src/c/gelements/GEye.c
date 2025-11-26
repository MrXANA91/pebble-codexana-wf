#include "GEye.h"

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_bitmap;
static GBitmap *s_xana_void_bitmap;
static GColor *s_xana_color;
static GColor *s_xana_void_color;

/// @brief Get the pointer to the black color in the bitmap palette
/// @param palette 
/// @param bitmap 
/// @attention Original code from [EurophoricPenguin/Fiftyeight](https://github.com/EuphoricPenguin/Fiftyeight/blob/main/src/c/fiftyeight.c)
static void prv_get_black_palette(GColor **palette_ptr_ptr, GBitmap* bitmap) {
  if (!palette_ptr_ptr) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No dest palette");
    return;
  }
  if (!bitmap) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No bitmap");
    return;
  }
  GColor *palette_array = gbitmap_get_palette(bitmap);
  if (!palette_array) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Bitmap has no palette");
    return;
  }
  // Get the bitmap format to determine palette size
  GBitmapFormat format = gbitmap_get_format(bitmap);
  int palette_size = 0;
  switch (format)
  {
    case GBitmapFormat1BitPalette:
      palette_size = 2;
      break;
    case GBitmapFormat2BitPalette:
      palette_size = 4;
      break;
    case GBitmapFormat4BitPalette:
      palette_size = 16;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Not a palette-based format, can't invert");
      return;
  }
  // Retrieving 
  for (int i = 0; i < palette_size; i++)
  {
    if (gcolor_equal(palette_array[i], GColorBlack))
    {
      *palette_ptr_ptr = &palette_array[i];
      return;
    }
  }
  APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to set palette");
  return;
}

void eye_init(Layer* window_layer, GRect bounds) {
  s_xana_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_FULL);
  s_xana_void_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA_VOID);
  prv_get_black_palette(&s_xana_color, s_xana_bitmap);
  prv_get_black_palette(&s_xana_void_color, s_xana_void_bitmap);
  s_xana_layer = bitmap_layer_create(GRect_XANA_default(bounds));
  bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  bitmap_layer_set_compositing_mode(s_xana_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_xana_layer));
}

void eye_update(bool connected) {
  ClaySettings *settings = settings_get();
  EyeModifiersEnum_t modifiers = connected ? settings->EyeOnConnected : settings->EyeOnDisconnected;
  if (modifiers == EYE_MODIFIER_NONE) {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
    *s_xana_color = settings->EyeColor;
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), false);
    return;
  }

  if ((modifiers & EYE_MODIFIER_HIDE) != 0) {
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), true);
  } else {
    layer_set_hidden(bitmap_layer_get_layer(s_xana_layer), false);
  }

  if ((modifiers & EYE_MODIFIER_VOID) != 0) {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_void_bitmap);
  } else {
    bitmap_layer_set_bitmap(s_xana_layer, s_xana_bitmap);
  }

  if ((modifiers & EYE_MODIFIER_GRAY) != 0) {
    if (s_xana_color) *s_xana_color = settings->EyeGrayedColor;
    if (s_xana_void_color) *s_xana_void_color = settings->EyeGrayedColor;
  } else {
    if (s_xana_color) *s_xana_color = settings->EyeColor;
    if (s_xana_void_color) *s_xana_void_color = settings->EyeColor;
  }
}

void eye_apply_settings(ClaySettings* settings) {
    bitmap_layer_set_background_color(s_xana_layer, settings->BackgroundColor);
    eye_update(true);
}

void eye_layout_update(GRect bounds, bool obstructed) {
    layer_set_frame(bitmap_layer_get_layer(s_xana_layer), obstructed ? GRect_XANA_obstructed(bounds) : GRect_XANA_default(bounds));
}

void eye_deinit() {
  gbitmap_destroy(s_xana_bitmap);
  gbitmap_destroy(s_xana_void_bitmap);
  bitmap_layer_destroy(s_xana_layer);
}