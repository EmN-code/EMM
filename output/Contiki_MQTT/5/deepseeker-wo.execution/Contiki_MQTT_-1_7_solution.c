
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
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
#include "dev/gpio-hal.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "string.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "sys/clock.h"
#include "net/ipv6/uip-nameserver.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static uint16_t mid;

static void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
  static struct etimer et;

  PROCESS_BEGIN();

  mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", event_callback, 128);
  mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (mqtt_connected(&conn)) {
      mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_0, 0, 0, 0, NULL);
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&mqtt_example_process, NULL);
  return 0;
}
