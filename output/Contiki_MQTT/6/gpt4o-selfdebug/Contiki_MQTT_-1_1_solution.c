#include "sys/node-id.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/linkaddr.h"
#include "dev/serial-line.h"
#include "lib/random.h"
#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID "nrf52840-mqtt-client"
#define TOPIC "device/state"

static struct etimer publish_timer;
static char pub_state[] = "work";
static struct mqtt_connection conn;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void
mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("MQTT connected\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("MQTT disconnected\n");
      process_exit(&mqtt_example_process);
      break;
    default:
      printf("MQTT event %d\n", event);
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, MQTT_CLIENT_ID, mqtt_event, 128);
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL * 3, MQTT_CLEAN_SESSION_ON);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
      mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)pub_state, strlen(pub_state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
      printf("Published state: %s\n", pub_state);

      // Toggle the state for demonstration
      if(strcmp(pub_state, "work") == 0) {
        strcpy(pub_state, "not work");
      } else {
        strcpy(pub_state, "work");
      }

      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}