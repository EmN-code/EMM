#include "net/ipv6/multicast/uip-mcast6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "random.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include <stdio.h>
#include <string.h>

PROCESS(mqtt_state_publish_process, "MQTT State Publish Process");
AUTOSTART_PROCESSES(&mqtt_state_publish_process);

static struct mqtt_connection conn;
static char client_id[32];
static char topic[64] = "nrf52840/state";
static struct etimer periodic_timer;
static char *broker_ip = "47.102.103.1";
static uint16_t broker_port = 1883;
static uint16_t RECONNECT_INTERVAL = 5;

void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to the MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from the MQTT broker\n");
      process_post(PROCESS_CURRENT(), PROCESS_EVENT_CONTINUE, NULL);
      break;
    default:
      break;
  }
}

PROCESS_THREAD(mqtt_state_publish_process, ev, data) {
  PROCESS_BEGIN();

  snprintf(client_id, sizeof(client_id), "nrf52840-client");

  mqtt_register(&conn, &mqtt_state_publish_process, client_id, mqtt_event_handler, MAX_TCP_SEGMENT_SIZE);
  mqtt_connect(&conn, broker_ip, broker_port, RECONNECT_INTERVAL);

  while (1) {
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if (mqtt_ready(&conn) && conn.out_buffer_sent) {
      static char message[20];
      snprintf(message, sizeof(message), "%s", "work");

      mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
      printf("Published state: %s\n", message);
    }
  }

  PROCESS_END();
}
