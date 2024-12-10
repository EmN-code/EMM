
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "mqtt.h"
#include "mqtt-transport.h"
#include "mqtt-packetbuf.h"
#include "mqtt-strings.h"
#include "mqtt-pal.h"
#include "mqtt-client.h"

#define LOG_MODULE "MQTT App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD_SIZE 20
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer publish_timer;

static void
mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
  if(event == MQTT_EVENT_CONNECT) {
    LOG_INFO("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("MQTT disconnected\n");
  }
}

static void
publish_state(void)
{
  static char payload[MQTT_PAYLOAD_SIZE];
  static uint16_t mid;

  snprintf(payload, MQTT_PAYLOAD_SIZE, "work");

  if(mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL) == MQTT_STATUS_SUCCESS) {
    LOG_INFO("Published state: %s\n", payload);
  } else {
    LOG_INFO("Failed to publish state\n");
  }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Starting MQTT client\n");

  uip_ipaddr_t broker_ip;
  uip_ipaddr(&broker_ip, 47, 102, 103, 1);

  mqtt_init(&mqtt_conn, MQTT_CLIENT_ID, NULL, NULL, mqtt_event_handler);
  mqtt_transport_init(&mqtt_conn, &udp_conn, &broker_ip, MQTT_BROKER_PORT);

  if(mqtt_connect(&mqtt_conn) == MQTT_STATUS_SUCCESS) {
    LOG_INFO("MQTT connection established\n");
  } else {
    LOG_INFO("MQTT connection failed\n");
  }

  etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    publish_state();
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}
