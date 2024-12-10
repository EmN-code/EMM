
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "lib/sensors.h"
#include "lib/dht11-sensor.h"
#include "lib/mqtt.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer et;

static void
mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *ptr)
{
  if(event == MQTT_EVENT_CONNECT) {
    printf("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECT) {
    printf("MQTT disconnected\n");
  }
}

static void
publish_state(void)
{
  static char state[] = "work";
  mqtt_publish(&mqtt_conn, MQTT_TOPIC, state, strlen(state), 0, 0, NULL);
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

  mqtt_init(&mqtt_conn, MQTT_CLIENT_ID, NULL, NULL, mqtt_event_handler);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, 0, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    publish_state();
    etimer_reset(&et);
  }

  PROCESS_END();
}
