#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

// Function to publish a message
static void
publish_state(void) {
  static char state[] = "work";
  mqtt_publish(&conn, NULL, "nrf52840/state", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

// Callback for MQTT events
static void
mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to the MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from the MQTT broker\n");
      break;
    case MQTT_EVENT_PUBLISH:
      printf("Published message\n");
      break;
    default:
      printf("Unhandled MQTT event: %d\n", event);
      break;
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publish_process, "nrf52840", mqtt_event, MAX_TCP_SEGMENT_SIZE);
  mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 30 * CLOCK_SECOND);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    publish_state();
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}
