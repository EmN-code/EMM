#include <stdio.h>
#include "contiki.h"
#include "mqtt.h"
#include "net/ipv6/uip-icmp6.h" // Included as given by alphabetical order requirement
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"

// Define the MQTT broker address and port
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(nrf52840_mqtt_client_process, "NRF52840 MQTT Client Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_client_process);

PROCESS_THREAD(nrf52840_mqtt_client_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &nrf52840_mqtt_client_process, "client_id");
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0); // Corrected usage

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    mqtt_publish(&conn, NULL, "status", "work", MQTT_QOS_LEVEL_0); // Corrected usage

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}