#include <pebble.h>

#include "config.h"

#include "windows/main_window.h"
#include "modules/settings.h"
#include "gelements/GDate.h"
#include "gelements/GClock.h"
#include "gelements/GEye.h"
#include "gelements/GBatteryLvl.h"
#include "gelements/GStepTracker.h"

static struct tm *current_time;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  current_time = localtime(&temp);

  clock_update(current_time);
  date_update(current_time);

  #ifdef DEBUG
  const int steps_counter_start = 0;
  static int fake_steps_counter = steps_counter_start;
  fake_steps_counter += 50;
  if (fake_steps_counter > (settings_get()->StepsBarMax + steps_counter_start)) {
    fake_steps_counter = steps_counter_start;
  }
  step_tracker_update(fake_steps_counter);
  #endif
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

static void health_callback(HealthEventType event, void *context) {
  if (event != HealthEventSignificantUpdate && event != HealthEventMovementUpdate) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Health event (%d) not relevant, ignoring", event);
    return;
  }

  HealthMetric metric = HealthMetricStepCount;

  time_t start = time_start_of_today();
  time_t end = time(NULL);
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
  if (mask & HealthServiceAccessibilityMaskAvailable) {
    #ifndef DEBUG
    step_tracker_update((int)health_service_sum_today(metric));
    #endif
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "HealthMetricStepCount not available");
  }
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

  #ifdef PBL_HEALTH
  if(!health_service_events_subscribe(health_callback, NULL)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  }
  #else
  APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  #endif

  // First time init values

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
