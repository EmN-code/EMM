
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state"

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer et;

static void
mqtt_event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data)
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
  static int state = 0;
  char payload[20];

  if(state == 0) {
    strcpy(payload, "work");
    state = 1;
  } else {
    strcpy(payload, "not work");
    state = 0;
  }

  mqtt_publish(&mqtt_conn, MQTT_TOPIC, payload, strlen(payload), 0, 0);
  printf("Published: %s\n", payload);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  PROCESS_BEGIN();

  uip_ipaddr_t broker_ip;
  uip_ipaddr(&broker_ip, 47, 102, 103, 1);

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 128);
  mqtt_connect(&mqtt_conn, &broker_ip, MQTT_BROKER_PORT, 0, 0);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    publish_state();
    etimer_reset(&et);
  }

  PROCESS_END();
}
