#include "net/routing/routing.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "sys/etimer.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

static char pub_topic[] = "device/status";
static char pub_message[20];

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Message published\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_process, CLIENT_ID, mqtt_event, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON); // Correct number of arguments

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        snprintf(pub_message, sizeof(pub_message), "State: %s", "work");
        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}