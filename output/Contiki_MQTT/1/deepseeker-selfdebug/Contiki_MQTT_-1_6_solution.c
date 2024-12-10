
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECT) {
    printf("Connected to MQTT broker\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    printf("Disconnected from MQTT broker\n");
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static uint16_t mid;

  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", event_callback, 128);
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, NULL);

  // Set up periodic timer
  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Publish message
    mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, sizeof(PAYLOAD) - 1, MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);

    // Reset timer
    etimer_reset(&et);
  }

  PROCESS_END();
}
