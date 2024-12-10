
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "mqtt.h"
#include "mqtt-prop.h"

#include <string.h>
#include <stdio.h>

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

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
  static char payload[32];
  static uint16_t mid;

  PROCESS_BEGIN();

  LOG_INFO("Starting MQTT example\n");

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_handler, 0);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 0, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (mqtt_connected(&mqtt_conn)) {
      strcpy(payload, "work");
      mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
