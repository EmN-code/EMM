#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "contiki.h"
#include "mqtt.h"
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

// Function to configure the MQTT connection
static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED: 
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker (reason: %u)\n", *((mqtt_event_t *)data));
      break;
    case MQTT_EVENT_PUBLISH: 
      printf("Message published\n");
      break;
    default:
      printf("MQTT Event: %d\n", event);
      break;
  }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publisher_process, "nrf_publisher", mqtt_event_callback, 100);
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 10000);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    if (conn.state == MQTT_CONN_STATE_CONNECTED) {
      uint16_t msg_id;
      char *topic = "device/state";
      char *message = "work";

      mqtt_publish(&conn, &msg_id, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0);
    }

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}

void main() {
  process_start(&mqtt_publisher_process, NULL);
}