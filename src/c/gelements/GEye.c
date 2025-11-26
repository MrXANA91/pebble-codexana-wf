#include "GEye.h"

static BitmapLayer *s_xana_layer;
static GBitmap *s_xana_bitmap;
static GBitmap *s_xana_void_bitmap;
static GColor *s_xana_color;
static GColor *s_xana_void_color;

static bool refreshed = false;
static void gray_xana_implementation_update(Animation *animation, const AnimationProgress progress) {
  if (!refreshed) {
    refreshed = true;
    if (s_xana_color) *s_xana_color = GColorLightGray;
    if (s_xana_void_color) *s_xana_void_color = GColorLightGray;
  }
}

static void restore_xana_implementation_update(Animation *animation, const AnimationProgress progress) {
  ClaySettings *settings = settings_get();
  if (!refreshed) {
    refreshed = true;
    if (s_xana_color) *s_xana_color = settings->EyeColor;
    if (s_xana_void_color) *s_xana_void_color = settings->EyeColor;
  }
}

static void empty_implementation_setup_teardown(Animation *animation) {
  refreshed = false;
}

void eye_start_animation() {
  GPoint start_finish = GPoint(0, 0);
  GPoint step_1 = GPoint(10, 0);
  GPoint step_2 = GPoint(-50, 0);
  GPoint step_3 = GPoint(50, 0);
  GPoint step_4 = GPoint(-20, 0);
  GPoint step_5 = GPoint(10, 0);

  const int delay_ms = 100;
  const int duration_ms = 50;
  const AnimationImplementation gray_xana_impl = {
    .setup = empty_implementation_setup_teardown,
    .update = gray_xana_implementation_update,
    .teardown = empty_implementation_setup_teardown
  };
  const AnimationImplementation restore_xana_impl = {
    .setup = empty_implementation_setup_teardown,
    .update = restore_xana_implementation_update,
    .teardown = empty_implementation_setup_teardown
  };
  
  PropertyAnimation *prop_anim_a = property_animation_create_bounds_origin(bitmap_layer_get_layer(s_xana_layer), &start_finish, &step_1);
  PropertyAnimation *prop_anim_c = property_animation_create_bounds_origin(bitmap_layer_get_layer(s_xana_layer), &step_1, &step_2);
  PropertyAnimation *prop_anim_d = property_animation_create_bounds_origin(bitmap_layer_get_layer(s_xana_layer), &step_2, &step_3);
  PropertyAnimation *prop_anim_e = property_animation_create_bounds_origin(bitmap_layer_get_layer(s_xana_layer), &step_3, &step_4);
  PropertyAnimation *prop_anim_f = property_animation_create_bounds_origin(bitmap_layer_get_layer(s_xana_layer), &step_4, &step_1);

  Animation *anim_a = property_animation_get_animation(prop_anim_a);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_a handle %d", (int)anim_a);
  animation_set_delay(anim_a, delay_ms);
  animation_set_duration(anim_a, duration_ms);
  animation_set_curve(anim_a, AnimationCurveLinear);
  Animation *anim_c = property_animation_get_animation(prop_anim_c);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_c handle %d", (int)anim_c);
  animation_set_duration(anim_c, duration_ms);
  animation_set_curve(anim_c, AnimationCurveLinear);
  Animation *anim_d = property_animation_get_animation(prop_anim_d);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_d handle %d", (int)anim_d);
  animation_set_duration(anim_d, duration_ms);
  animation_set_curve(anim_d, AnimationCurveLinear);
  Animation *anim_e = property_animation_get_animation(prop_anim_e);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_e handle %d", (int)anim_e);
  animation_set_duration(anim_e, duration_ms);
  animation_set_curve(anim_e, AnimationCurveLinear);
  Animation *anim_f = property_animation_get_animation(prop_anim_f);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_f handle %d", (int)anim_f);
  animation_set_duration(anim_f, duration_ms);
  animation_set_curve(anim_f, AnimationCurveLinear);
  Animation *anim_h = animation_clone(anim_a);
  APP_LOG(APP_LOG_LEVEL_INFO, "Retrieved anim_h handle %d", (int)anim_h);
  animation_set_reverse(anim_h, true);
  animation_set_delay(anim_h, 0);

  Animation *anim_b = animation_create();
  if (!anim_b) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_b is null");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Created anim_b handle %d", (int)anim_b);
  }
  if (animation_set_implementation(anim_b, &gray_xana_impl) == false) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_b set impl failed");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "anim_b set impl ok");
  }
  if (animation_set_duration(anim_b, 1) == false) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_b set duration failed");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "anim_b set duration ok");
  }
  Animation *anim_g = animation_create();
  if (!anim_g) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_b is null");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Created anim_g handle %d", (int)anim_g);
  }
  if (animation_set_implementation(anim_g, &restore_xana_impl) == false) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_g set impl failed");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "anim_g set impl ok");
  }
  if (animation_set_duration(anim_g, 1) == false) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "anim_g set duration failed");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "anim_g set duration ok");
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "Creating sequence...");
  // const uint32_t array_length = 7;
  // Animation **arr = (Animation**)malloc(array_length * sizeof(Animation*));
  // arr[0] = anim_a;
  // arr[1] = anim_b;
  // arr[2] = anim_c;
  // arr[3] = anim_d;
  // arr[4] = anim_e;
  // arr[5] = anim_f;
  // arr[6] = anim_g;
  Animation *sequence = animation_sequence_create(anim_a, anim_c, anim_d, anim_e, anim_f, anim_h);
  // Animation *sequence = animation_sequence_create_from_array(arr, array_length);
  if (!sequence) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "sequence is null");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Created sequence handle %d", (int)anim_g);
  }

  if (animation_schedule(sequence) == false) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to launch sequence");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Sequence launched");
  }

  // free(arr);
}

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
  s_xana_bitmap = gbitmap_create_with_resource(RESOURCE_ID_XANA);
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