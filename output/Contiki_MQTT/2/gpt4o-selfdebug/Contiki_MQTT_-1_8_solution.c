#include "net/ipv6/uiplib.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static struct mqtt_connection conn;
static struct etimer periodic_timer;

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840_mqtt_client"
#define MQTT_PUBLISH_TOPIC "device/state"

PROCESS(nrf52840_mqtt_client_process, "nRF52840 MQTT Client Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_client_process);

PROCESS_THREAD(nrf52840_mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    static char pub_message[20];
    mqtt_status_t status;
    uint16_t mid;

    mqtt_register(&conn, &nrf52840_mqtt_client_process, MQTT_CLIENT_ID, mqtt_event_handler_cb, 512);

    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                 (PUB_INTERVAL << 1) / CLOCK_SECOND,
                 MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, PUB_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        strcpy(pub_message, "work");
        status = mqtt_publish(&conn, &mid, MQTT_PUBLISH_TOPIC,
                              (uint8_t *)pub_message, strlen(pub_message),
                              MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        if (status != MQTT_STATUS_OK) {
            printf("Failed to publish the message, status: %d\n", status);
        }

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}

void mqtt_event_handler_cb(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED: {
            printf("MQTT connection established\n");
            break;
        }
        case MQTT_EVENT_DISCONNECTED: {
            printf("MQTT connection disconnected\n");
            break;
        }
        case MQTT_EVENT_PUBLISH_ACK: {
            printf("Publish acknowledged\n");
            break;
        }
        default: {
            printf("MQTT event occurred, event id: %d\n", event);
            break;
        }
    }
}