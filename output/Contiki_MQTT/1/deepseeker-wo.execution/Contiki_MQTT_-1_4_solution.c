
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "lib/random.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection mqtt_conn;
static struct etimer et;

void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECT) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    printf("MQTT disconnected\n");
  }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static char message[20];
  static int state = 0;

  PROCESS_BEGIN();

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_handler, 128);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    state = !state;
    snprintf(message, sizeof(message), "State: %s", state ? "work" : "not work");

    mqtt_publish(&mqtt_conn, MQTT_TOPIC, message, strlen(message), 0, 0);

    etimer_reset(&et);
  }

  PROCESS_END();
}
