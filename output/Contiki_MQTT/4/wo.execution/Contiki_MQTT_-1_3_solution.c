#include <stdio.h>
#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "mqtt.h"
#include "net/ipv6/simple-udp.h"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define PUB_INTERVAL 5

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data){
  switch (event) {
     case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT broker\n");
        break;
     case MQTT_EVENT_DISCONNECTED:
        printf("Disconnected from MQTT broker\n");
        break;
     default:
        printf("MQTT event: %d\n", event);
        break;
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data){
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, "client-id", mqtt_event_handler, MQTT_BROKER_PORT);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 240);

  etimer_set(&periodic_timer, CLOCK_SECOND * PUB_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    char *state = "work"; // here just an example state
    mqtt_publish(&conn, NULL, STATE_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}