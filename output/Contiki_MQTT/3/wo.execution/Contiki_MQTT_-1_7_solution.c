#include "dev/button-hal.h"
#include "dht11-sensor.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "random.h"
#include <string.h>
#include <stdio.h>
#include "sys/etimer.h"
#include "mqtt-prop.h"
#include "mqtt.h"

static struct mqtt_connection conn;
static struct etimer periodic_timer; // Timer for periodic execution
static char client_id[32];
static char pub_topic[] = "device/state";
static char message[32];

PROCESS(example_mqtt_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&example_mqtt_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
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

PROCESS_THREAD(example_mqtt_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &example_mqtt_process, client_id, mqtt_event, 128);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            etimer_reset(&periodic_timer);

            if(!mqtt_connected(&conn)) {
                printf("Reconnecting to MQTT broker\n");
                mqtt_connect(&conn, "47.102.103.1", 1883, 60);
            } else {
                strncpy(message, (random_rand() % 2) ? "work" : "not work", sizeof(message));
                mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
                printf("Published message: %s\n", message);
            }
        }
    }

    PROCESS_END();
}