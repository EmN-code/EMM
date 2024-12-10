#include "net/linkaddr.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include <stdio.h>
#include "mqtt.h"
#include "sys/etimer.h"
#include "sys/log.h"

#define LOG_MODULE "MQTT-Example"
#define LOG_LEVEL LOG_LEVEL_INFO

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
  case MQTT_EVENT_CONNECTED:
    LOG_INFO("MQTT connection established\n");
    mqtt_publish(&conn, NULL, "nrf52840/state", (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    break;
  case MQTT_EVENT_DISCONNECTED:
    LOG_INFO("MQTT disconnected\n");
    break;
  default:
    break;
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, NULL, mqtt_event, 128);

  mqtt_connect(&conn, "47.102.103.1", 1883, 60 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);

  etimer_set(&periodic_timer, 5 * CLOCK_SECOND);
  while (1) {
    PROCESS_YIELD();
    if (etimer_expired(&periodic_timer)) {
      mqtt_publish(&conn, NULL, "nrf52840/state", (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}