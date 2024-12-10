// Necessary includes in descending alphabetical order by first letter
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip.h"
#include "contiki-default-conf.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "coap.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/mac/framer/framer.h"
#include "contiki-net.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "mqtt.h"

// Define constants
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND) // publish every 5 seconds

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_demo_process, "MQTT Demo Process");
AUTOSTART_PROCESSES(&mqtt_demo_process);

static void mqtt_event_callback(struct mqtt_connection *m,
                                mqtt_event_t event,
                                void *data) {
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker");
      break;
    default:
      break;
  }
}

PROCESS_THREAD(mqtt_demo_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_demo_process, "nrf52840", mqtt_event_callback, 256);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60 * CLOCK_SECOND, 0, NULL);

  etimer_set(&periodic_timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&periodic_timer)) {
      static char message[] = "work";
      uint16_t msg_id;

      mqtt_publish(&conn, &msg_id, "state", (uint8_t *)message, strlen(message),
                   MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);

      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}