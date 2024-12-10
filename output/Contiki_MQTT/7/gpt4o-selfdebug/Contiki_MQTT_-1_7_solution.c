#include "sensors.h"
#include <net/ipv6/udp-socket.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/uip-ds6-route.h>
#include <net/ipv6/uip-ds6-nbr.h>
#include <net/ipv6/uip-icmp6.h>
#include <stdio.h>
#include <stdlib.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char client_id[BUFFER_SIZE];
static char topic[BUFFER_SIZE];
static char message[BUFFER_SIZE];

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    }
    else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the MQTT connection
    mqtt_register(&conn, &mqtt_example_process, client_id, mqtt_event_handler, BUFFER_SIZE);

    // Establish the MQTT connection
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        // Prepare the message
        sprintf(message, "work"); // For demonstration, assuming it always publishes "work"

        // Publish the message
        mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}