
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_connected(struct mqtt_connection *c) {
  printf("MQTT connected\n");
}

static void mqtt_disconnected(struct mqtt_connection *c) {
  printf("MQTT disconnected\n");
}

static void mqtt_published(struct mqtt_connection *c, uint16_t mid) {
  printf("Message published, mid: %u\n", mid);
}

static void mqtt_data_received(struct mqtt_connection *c, const char *topic, const uint8_t *payload, uint16_t payload_len) {
  printf("Data received on topic %s: %.*s\n", topic, payload_len, payload);
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_init(&conn, NULL);
  conn.connected_cb = mqtt_connected;
  conn.disconnected_cb = mqtt_disconnected;
  conn.published_cb = mqtt_published;
  conn.data_cb = mqtt_data_received;

  // Connect to MQTT broker
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  // Set up periodic timer
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Publish message
    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_MESSAGE, sizeof(MQTT_MESSAGE) - 1, MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);

    // Reset timer
    etimer_reset(&et);
  }

  PROCESS_END();
}
