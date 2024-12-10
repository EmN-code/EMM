
#include "contiki-default-conf.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "string.h"
#include "random.h"
#include "net/mac/mac.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-conf.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "string.h"
#include "net/routing/routing.h"
#include "contiki-lib.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "coap-engine.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char payload[20];

static void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  static struct etimer et;
  static uint16_t mid;

  PROCESS_BEGIN();

  mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_handler, 0);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 0, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    strcpy(payload, "work");
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), 0, 0, 0, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&mqtt_publisher_process, NULL);
  return 0;
}
