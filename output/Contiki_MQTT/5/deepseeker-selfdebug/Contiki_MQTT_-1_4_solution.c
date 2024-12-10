
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static char message[] = "work";
  static uint16_t mid;

  PROCESS_BEGIN();

  mqtt_init(&conn, NULL);
  mqtt_set_event_handler(&conn, mqtt_event_handler);

  while (1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)message, sizeof(message) - 1, MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
    mqtt_disconnect(&conn, NULL);
  }

  PROCESS_END();
}
