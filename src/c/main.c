#include <pebble.h>

#include "config.h"

#include "windows/main_window.h"
#include "modules/settings.h"
#include "gelements/GDate.h"
#include "gelements/GClock.h"
#include "gelements/GEye.h"
#include "gelements/GBatteryLvl.h"

static struct tm *current_time;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  current_time = localtime(&temp);

  clock_update(current_time);
  date_update(current_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void bluetooth_callback(bool connected) {
  eye_update(connected);

  ClaySettings *settings = settings_get();
  if (settings->VibrateOnDisconnect && !connected) {
    vibes_double_pulse();
  }
}

static void battery_callback(BatteryChargeState batteryCharge) {
  // Updating the battery level
  battery_lvl_update(batteryCharge.charge_percent);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  eye_start_animation();
}

static void prv_init(void) {
  main_window_push();

  tick_timer_service_subscribe(
  #ifdef DEBUG
  SECOND_UNIT
  #else
  MINUTE_UNIT
  #endif
  , tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  
  battery_state_service_subscribe(battery_callback);

  accel_tap_service_subscribe(accel_tap_handler);

  update_time();

  bluetooth_callback(connection_service_peek_pebble_app_connection());

  battery_callback(battery_state_service_peek());
}

static void prv_deinit(void) {
  main_window_destroy();
}

int main(void) {
  settings_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
