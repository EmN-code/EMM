
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
static uint8_t payload[20];

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    LOG_INFO("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
  static struct etimer et;
  static uint16_t mid;

  PROCESS_BEGIN();

  LOG_INFO("Starting MQTT example\n");

  mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 128);

  while (1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (mqtt_connected(&conn)) {
      strcpy((char *)payload, "work");
      mqtt_publish(&conn, &mid, MQTT_TOPIC, payload, strlen((char *)payload), MQTT_QOS_LEVEL_0);
    } else {
      mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60);
    }
  }

  PROCESS_END();
}
