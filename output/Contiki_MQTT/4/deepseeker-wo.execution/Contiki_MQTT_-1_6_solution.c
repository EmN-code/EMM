
#include "contiki-default-conf.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
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
#include "contiki-default-conf.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "mqtt-prop.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-conf.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "net/routing/routing.h"
#include "contiki-lib.h"
#include "sys/ctimer.h"
#include "mqtt.h"
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

static struct mqtt_connection conn;
static struct etimer et;

void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

void publish_state(void) {
  static int state = 0;
  char payload[20];
  sprintf(payload, "State: %s", state ? "work" : "not work");
  mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, NULL);
  state = !state;
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_handler, 128);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    publish_state();
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&mqtt_client_process, NULL);
  return 0;
}
