#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "mqtt.h"
#include <stdio.h>
#include "contiki.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct etimer publish_timer;
static struct mqtt_connection conn;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, "nrf52840_client", NULL, 123);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60 * CLOCK_SECOND);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    if(conn.state == MQTT_CONN_STATE_CONNECTED) {
      uint16_t mid;
      mqtt_publish(&conn, &mid, "device/state", (uint8_t *)"work", 4, MQTT_QOS_LEVEL_0);
      printf("Published state to MQTT broker\n");
    } else {
      printf("Not connected to MQTT broker\n");
    }
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&mqtt_example_process, NULL);
  return 0;
}