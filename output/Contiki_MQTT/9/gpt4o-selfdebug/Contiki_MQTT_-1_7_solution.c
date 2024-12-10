#include <stdio.h>
#include "mqtt.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-udp-packet.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)  // Publish every 5 seconds

static struct mqtt_connection conn;
static char *pub_topic = "device/state";
static uint8_t pub_payload_on[] = "work";
static uint8_t pub_payload_off[] = "not work";

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker\n");
      break;
    default:
      break;
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, MQTT_BROKER_IP, mqtt_event, MAX_TCP_SEGMENT_SIZE);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, PUBLISH_INTERVAL / CLOCK_SECOND);

  etimer_set(&periodic_timer, PUBLISH_INTERVAL);

  while (1) {
    uint16_t mid;
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    mqtt_publish(&conn, &mid, pub_topic, pub_payload_on, sizeof(pub_payload_on), MQTT_QOS_LEVEL_0);
    printf("Published \"work\" to %s\n", pub_topic);

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}