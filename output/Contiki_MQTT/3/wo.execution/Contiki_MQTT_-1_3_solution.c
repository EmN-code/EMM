#include "net/routing/routing.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "net/netstack.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "udp-socket.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer timer;

PROCESS(mqtt_demo, "MQTT demo process");
AUTOSTART_PROCESSES(&mqtt_demo);

PROCESS_THREAD(mqtt_demo, ev, data)
{
  PROCESS_BEGIN();

  static char pub_topic[] = "work_status";
  static char pub_payload[] = "work";

  mqtt_register(&conn, &mqtt_demo, "nrf52840_publisher", NULL, 64);

  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60);

  etimer_set(&timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&timer)) {
      mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_payload, sizeof(pub_payload), 0, 0, 0);
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}