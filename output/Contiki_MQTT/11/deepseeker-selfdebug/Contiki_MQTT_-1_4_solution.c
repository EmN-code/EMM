
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  static uint16_t mid;
  static char payload[] = MQTT_PAYLOAD;

  PROCESS_BEGIN();

  mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", event_callback, 128);
  mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, sizeof(payload) - 1, MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}
