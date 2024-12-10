#include "sys/process.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "mqtt.h"
#include "net/ipv6/multicast/uip-mcast6.h"
#include "contiki.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_demo_process, "MQTT Demo Process");
AUTOSTART_PROCESSES(&mqtt_demo_process);

static struct etimer publish_timer;
static struct mqtt_connection mqtt_conn;
static char *topic = "nrf52840/status";

// Callback function for MQTT event handling
static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker\n");
      break;
    case MQTT_EVENT_PUBLISH:
      printf("MQTT message published\n");
      break;
    default:
      break;
  }
}

PROCESS_THREAD(mqtt_demo_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize the MQTT connection
  mqtt_register(&mqtt_conn, &mqtt_demo_process, CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);
  mqtt_connect(&mqtt_conn, BROKER_IP, BROKER_PORT, MQTT_PUBLISH_INTERVAL);

  etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER) {
      char payload[] = "work or not work";
      mqtt_publish(&mqtt_conn, NULL, topic, (uint8_t *)payload, strlen(payload),
                    MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

      // Reset the timer for the next publish event
      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}