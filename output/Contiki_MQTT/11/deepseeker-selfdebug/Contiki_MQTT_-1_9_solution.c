
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_init(&conn, NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT, event_callback, NULL);

  // Connect to MQTT broker
  mqtt_connect(&conn, NULL, NULL, NULL, 0, NULL);

  // Set timer for periodic publishing
  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Publish message
    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_0, 0, 0, 0, NULL);

    // Reset timer
    etimer_reset(&et);
  }

  PROCESS_END();
}
