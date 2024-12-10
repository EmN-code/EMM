#include "sensors.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static struct etimer pub_timer;
static char client_id[20];

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to the MQTT broker!\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from the MQTT broker!\n");
      break;
    case MQTT_EVENT_PUBLISH:
      printf("Message published successfully!\n");
      break;
    default:
      printf("MQTT Event: %d\n", event);
      break;
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  snprintf(client_id, sizeof(client_id), "nrf52840-client");
  mqtt_register(&conn, &mqtt_publish_process, client_id, mqtt_event, 256);

  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60);

  etimer_set(&pub_timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&pub_timer)) {
      static char message[30];
      snprintf(message, sizeof(message), "State: work");
      mqtt_publish(&conn, NULL, "nrf52840/state", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0);
      etimer_reset(&pub_timer);
    }
  }

  PROCESS_END();
}