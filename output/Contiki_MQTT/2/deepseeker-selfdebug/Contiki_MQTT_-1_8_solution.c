
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
#include "mqtt-packet.h"
#include "mqtt-strings.h"
#include "mqtt-client.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer publish_timer;

static void
mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data)
{
  if (event == MQTT_EVENT_CONNECT) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("MQTT disconnected\n");
  }
}

static void
publish_state(void)
{
  static uint8_t state = 0;
  char payload[20];
  uint16_t mid;

  state = !state;
  snprintf(payload, sizeof(payload), "State: %s", state ? "work" : "not work");

  mqtt_status_t status = mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
  if (status == MQTT_STATUS_SUCCESS) {
    LOG_INFO("Published: %s\n", payload);
  } else {
    LOG_INFO("Failed to publish\n");
  }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  mqtt_init(&mqtt_conn, MQTT_CLIENT_ID, NULL, NULL, mqtt_event_handler);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, NULL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    etimer_reset(&publish_timer);
    publish_state();
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&mqtt_publisher_process, NULL);
  return 0;
}
