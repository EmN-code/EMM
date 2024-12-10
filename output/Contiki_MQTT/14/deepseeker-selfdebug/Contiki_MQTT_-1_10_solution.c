
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "net/ip/tcpip.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "mqtt.h"
#include "mqtt-transport.h"
#include "mqtt-process.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection mqtt_conn;
static struct etimer publish_timer;

static void
mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data)
{
  if(event == MQTT_EVENT_CONNECT) {
    printf("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECT) {
    printf("MQTT disconnected\n");
  }
}

static void
mqtt_publish_state(void)
{
  uint16_t mid;
  mqtt_status_t status;

  status = mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, MQTT_PAYLOAD_SIZE, MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
  if(status == MQTT_STATUS_OK) {
    printf("Published state: %s\n", MQTT_PAYLOAD);
  } else {
    printf("Failed to publish state\n");
  }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  PROCESS_BEGIN();

  static struct mqtt_prop_list prop_list;
  static struct mqtt_prop_out_property prop_out;

  mqtt_prop_list_init(&prop_list);
  mqtt_prop_register_internal(&prop_list, &prop_out, MQTT_MSG_TYPE_CONNECT, MQTT_PROP_ID_SESSION_EXPIRY_INTERVAL, NULL);

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840-client", mqtt_event_callback, 1024);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, &prop_list);

  etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    mqtt_publish_state();
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}
