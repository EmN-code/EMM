#include "mqtt.h"
#include "ctimer.h"
#include "contiki.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static struct ctimer publish_timer;

static void publish_callback(void *data) {
  static int state = 0;
  char *state_str = state ? "work" : "not work";
  uint16_t mid;

  mqtt_publish(&conn, &mid, "state/topic", (uint8_t *)state_str, strlen(state_str), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_NOT_SET, NULL);

  state = !state;
  ctimer_reset(&publish_timer);
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publisher_process, "client_id", NULL, 64);

  mqtt_connect(&conn, "47.102.103.1", 1883, 60, 0, NULL);

  ctimer_set(&publish_timer, CLOCK_SECOND * 5, publish_callback, NULL);

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  PROCESS_END();
}