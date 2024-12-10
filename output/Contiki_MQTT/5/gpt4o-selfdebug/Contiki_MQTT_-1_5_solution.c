#include <string.h>
#include <stdio.h>
#include "net/ipv6/multicast/uip-mcast6.h"
#include "mqtt.h"
#include "contiki.h"
#include "sys/etimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer timer;
static char client_id[] = CLIENT_ID;
static char publish_payload[20];

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

mqtt_event_callback_t mqtt_event_handler;

void mqtt_event_handler(struct mqtt_connection *connection, mqtt_event_t event, void *data) {
  // Handle MQTT events here
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connection established\n");
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, client_id, mqtt_event_handler, 1024);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 5 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);
    
    etimer_set(&timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        strcpy(publish_payload, "work");

        mqtt_publish(&conn, NULL, PUBLISH_TOPIC, publish_payload, strlen(publish_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&timer);
    }

    PROCESS_END();
}