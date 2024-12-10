
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/gpio-hal.h"
#include "random.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/psock.h"
#include "net/routing/routing.h"
#include "mqtt-prop.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection conn;
static uint16_t mid;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event) {
  if (event == MQTT_EVENT_CONNECTED) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    LOG_INFO("MQTT disconnected\n");
  }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  static struct etimer et;
  static char payload[16];

  PROCESS_BEGIN();

  LOG_INFO("Starting MQTT example\n");

  mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    snprintf(payload, sizeof(payload), "work");
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_NONE, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}
