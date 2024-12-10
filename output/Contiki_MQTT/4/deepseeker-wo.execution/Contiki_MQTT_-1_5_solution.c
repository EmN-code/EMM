
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct simple_udp_connection udp_conn;

void mqtt_event_callback(void *arg, int event, void *data) {
  (void)arg;
  (void)data;
  if (event == 0) {
    printf("MQTT Connected\n");
  } else {
    printf("MQTT Disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static struct etimer et;
  static char payload[10];

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, 0, NULL, 0, NULL);

  while (1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    strcpy(payload, MQTT_PAYLOAD);
    simple_udp_sendto(&udp_conn, payload, strlen(payload), (uip_ipaddr_t *)MQTT_BROKER_IP, MQTT_BROKER_PORT);

    printf("Published: %s\n", payload);
  }

  PROCESS_END();
}

int main() {
  process_start(&mqtt_publish_process, NULL);
  return 0;
}
