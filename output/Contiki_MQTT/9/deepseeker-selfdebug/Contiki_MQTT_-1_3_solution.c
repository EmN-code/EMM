
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "net/ip/mqtt.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void
mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data)
{
  if(event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  static uint16_t mid;
  static char payload[MQTT_PAYLOAD_SIZE];

  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);

  // Set up periodic timer
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Reset timer
    etimer_reset(&et);

    // Publish message
    strncpy(payload, MQTT_PAYLOAD, MQTT_PAYLOAD_SIZE);
    mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, MQTT_PAYLOAD_SIZE, MQTT_QOS, MQTT_RETAIN, 0, 0, NULL);
  }

  PROCESS_END();
}
