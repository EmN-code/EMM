#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883
#define CLIENT_ID      "nrf52840"
#define MQTT_PUBLISH_TOPIC "nrf52840/status"
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char buf[64];

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("MQTT connection established\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("MQTT connection disconnected\n");
      break;
    case MQTT_EVENT_PUBLISH: {
      printf("MQTT message published\n");
      break;
    }
    case MQTT_EVENT_SUBACK:
    case MQTT_EVENT_UNSUBACK:
    case MQTT_EVENT_PUBACK:
      break;
    case MQTT_EVENT_ERROR:
      printf("MQTT connection error\n");
      break;
    default:
      printf("Unhandled MQTT event %d\n", event);
      break;
  }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);

  printf("Connecting to MQTT broker\n");
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, MQTT_PUBLISH_INTERVAL * 3 / CLOCK_SECOND);

  etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT();

    if (ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
      snprintf(buf, sizeof(buf), "work"); // Change this to "not work" if needed.
      printf("Publishing MQTT message: %s\n", buf);
      mqtt_publish(&conn, NULL, MQTT_PUBLISH_TOPIC, (uint8_t *)buf, strlen(buf), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}