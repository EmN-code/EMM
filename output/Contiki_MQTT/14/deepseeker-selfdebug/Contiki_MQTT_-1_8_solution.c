
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/tcpip.h"
#include "net/ip/mqtt.h"
#include "dev/leds.h"
#include "dev/uart0.h"  // Include the correct UART header file

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection mqtt_conn;
static char client_id[] = "nrf52840_client";

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

static void publish_state(void) {
  static int state = 0;
  char payload[20];

  if (state) {
    strcpy(payload, "work");
  } else {
    strcpy(payload, "not work");
  }

  state = !state;

  mqtt_publish(&mqtt_conn, NULL, MQTT_TOPIC, payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, NULL);
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();

  tcpip_init(NULL, NULL);

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), client_id, mqtt_event_callback, 1024);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, NULL);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    publish_state();

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&mqtt_publish_process, NULL);
  return 0;
}
