
#include "contiki-default-conf.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "string.h"
#include "net/mac/mac.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-conf.h"
#include "net/netstack.h"
#include "net/ipv6/uip-sr.h"
#include "contiki-lib.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/timer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "sys/log.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/netstack.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection conn;
static uint8_t mqtt_payload[10];

static void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event) {
  if (event == MQTT_EVENT_CONNECTED) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    LOG_INFO("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
  static struct etimer et;
  static uint8_t state = 0;

  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 5);

  mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_handler, 128);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (state == 0) {
      strncpy((char *)mqtt_payload, MQTT_PAYLOAD_WORK, sizeof(mqtt_payload));
      state = 1;
    } else {
      strncpy((char *)mqtt_payload, MQTT_PAYLOAD_NOT_WORK, sizeof(mqtt_payload));
      state = 0;
    }

    mqtt_publish(&conn, NULL, MQTT_TOPIC, mqtt_payload, strlen((char *)mqtt_payload), MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&mqtt_client_process, NULL);
  return 0;
}