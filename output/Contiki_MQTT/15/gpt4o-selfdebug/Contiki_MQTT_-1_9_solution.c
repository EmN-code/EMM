#include "stdlib.h"
#include "stdio.h"
#include "contiki.h"
#include "mqtt.h"
#include "net/ipv6/simple-udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840-client"

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char state_msg[20];

PROCESS(nrf52840_mqtt_client, "NRF52840 MQTT Client");
AUTOSTART_PROCESSES(&nrf52840_mqtt_client);

static void mqtt_event_callback(struct mqtt_connection *m,
                                mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to the broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from the broker\n");
      break;
    case MQTT_EVENT_PUBLISH:
      printf("Message published\n");
      break;
    default:
      printf("Unknown event %d\n", event);
      break;
  }
}

PROCESS_THREAD(nrf52840_mqtt_client, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &nrf52840_mqtt_client, MQTT_CLIENT_ID, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);

  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, (mqtt_keep_alive_t)10 * CLOCK_SECOND, 1);

  etimer_set(&publish_timer, 5 * CLOCK_SECOND);

  while (1) {
    PROCESS_YIELD();

    if (etimer_expired(&publish_timer)) {
      // Compose the message
      snprintf(state_msg, sizeof(state_msg), "work");
      // Prepare the message id
      uint16_t msg_id;

      mqtt_publish(&conn, &msg_id, "device/state", (uint8_t *)state_msg,
                   strlen(state_msg), MQTT_QOS_LEVEL_0,
                   MQTT_RETAIN_OFF);
      printf("State published: %s\n", state_msg);

      // Reset the timer for the next publish
      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}
