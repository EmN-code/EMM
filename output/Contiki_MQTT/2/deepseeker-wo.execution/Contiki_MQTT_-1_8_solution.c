
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "net/netstack.h"
#include "dev/gpio-hal.h"
#include "mqtt.h"
#include "string.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECT) {
    LOG_INFO("Connected to MQTT broker\n");
  } else if (event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("Disconnected from MQTT broker\n");
  }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
  static char payload[20];
  static uint16_t mid;

  PROCESS_BEGIN();

  mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 128);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (mqtt_connected(&conn)) {
      strcpy(payload, "work");
      mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0);
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
