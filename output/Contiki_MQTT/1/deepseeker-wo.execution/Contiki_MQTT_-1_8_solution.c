
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "string.h"
#include "net/ipv6/tcp-socket.h"
#include "dev/leds.h"
#include "net/ipv6/tcpip.h"
#include "contiki-conf.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/netstack.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define LOG_MODULE "MQTT App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

void mqtt_event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECT) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("MQTT disconnected\n");
  }
}

void publish_state(void) {
  static int state = 0;
  char payload[12];
  uint16_t mid;
  mqtt_status_t status;

  snprintf(payload, sizeof(payload), "work %s", state ? "not work" : "work");
  state = !state;

  status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
  if (status == MQTT_STATUS_SUCCESS) {
    LOG_INFO("Published: %s\n", payload);
  } else {
    LOG_ERR("Failed to publish\n");
  }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 0);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 0, NULL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    publish_state();
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&mqtt_client_process, NULL);
  return 0;
}
