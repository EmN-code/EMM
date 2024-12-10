#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/etimer.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"
#define UPDATE_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer periodic_timer;
static struct mqtt_connection conn;

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    static char client_id[] = MQTT_CLIENT_ID;
    static char broker_address[] = MQTT_BROKER_IP;
    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event, MAX_TCP_SEGMENT_SIZE);

    etimer_set(&periodic_timer, UPDATE_INTERVAL);
    
    static mqtt_status_t status;
    printf("Connecting to MQTT broker at %s:%d\n", broker_address, MQTT_BROKER_PORT);
    status = mqtt_connect(&conn, broker_address, MQTT_BROKER_PORT, 60 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);

    if(status != MQTT_STATUS_OK) {
        printf("MQTT connection failed.\n");
        PROCESS_EXIT();
    }

    while(1) {
        PROCESS_YIELD();
        if(ev == PROCESS_EVENT_TIMER) {
            if(etimer_expired(&periodic_timer)) {
                const char *state_msg = "work";  // Example state message
                status = mqtt_publish(&conn, NULL, MQTT_PUBLISH_TOPIC, (uint8_t *)state_msg, strlen(state_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
                if(status == MQTT_STATUS_OK) {
                    printf("Published message: %s\n", state_msg);
                } else {
                    printf("Failed to publish message.\n");
                }
                etimer_reset(&periodic_timer);
            }
        }
    }

    PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected.\n");
        process_exit(&mqtt_publisher_process);
    }
}