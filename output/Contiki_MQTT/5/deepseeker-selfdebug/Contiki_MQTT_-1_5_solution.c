
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include <stdio.h>
#include <string.h>

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection conn;
static struct etimer et;

static void
mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data)
{
  if (event == MQTT_EVENT_CONNECTED) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    LOG_INFO("MQTT disconnected\n");
  }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  static uint16_t mid;
  static char state[] = "work";
  static int toggle = 0;

  PROCESS_BEGIN();

  LOG_INFO("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

  mqtt_init(&conn, NULL);
  mqtt_set_event_handler(&conn, mqtt_event_handler);

  if (mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL) == MQTT_STATUS_SUCCESS) {
    LOG_INFO("MQTT connection established\n");
  } else {
    LOG_INFO("MQTT connection failed\n");
    PROCESS_EXIT();
  }

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (toggle) {
      strcpy(state, "work");
      toggle = 0;
    } else {
      strcpy(state, "not work");
      toggle = 1;
    }

    LOG_INFO("Publishing state: %s\n", state);
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}
