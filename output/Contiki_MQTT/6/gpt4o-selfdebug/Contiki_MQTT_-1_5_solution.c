#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>

static struct mqtt_connection conn;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  static struct etimer timer;
  static char *broker_ip = "47.102.103.1";
  static uint16_t broker_port = 1883;
  static char *pub_topic = "state";
  static uint16_t keep_alive = 30;
  static uint8_t clean_session = 1;
  static uint16_t msg_id;
  static char *message = "work";
  static struct mqtt_prop_list *prop_list = NULL;
  
  PROCESS_BEGIN();

  mqtt_connect(&conn, broker_ip, broker_port, keep_alive, clean_session, prop_list);

  etimer_set(&timer, 5 * CLOCK_SECOND);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&timer)) {
      mqtt_publish(&conn, &msg_id, pub_topic, (uint8_t *)message, strlen(message), 0, 0, 0, 0, prop_list);
      etimer_reset(&timer);
    }
  }
  
  PROCESS_END();
}