
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "lib/random.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "dev/leds.h"
#include "dev/button-hal.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "coap-blocking-api.h"
#include "coap-engine.h"
#include "coap-log.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection mqtt_conn;
static struct etimer et;

static void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECT) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("MQTT disconnected\n");
  }
}

static void publish_state(void) {
  static int state = 0;
  char payload[20];
  uint16_t mid;

  state = !state;
  snprintf(payload, sizeof(payload), "State: %s", state ? "work" : "not work");

  mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0);
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 5);

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_handler, 128);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);

    publish_state();
  }

  PROCESS_END();
}

int main(void) {
  process_start(&mqtt_example_process, NULL);
  return 0;
}
