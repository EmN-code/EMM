#include <stdio.h>
#include <stdlib.h>
#include "mqtt.h"
#include "uip.h"
#include "uip-ds6-route.h"
#include "mqtt-util.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840"
#define PUBLISH_INTERVAL 5
#define BUFFER_SIZE 64

static struct mqtt_connection conn;
static char client_id[] = CLIENT_ID;
static char topic[] = "device/state";
static char message[] = "work";

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to the broker\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("Disconnected from the broker\n");
        break;
    default:
        break;
    }
}

int main(void) {
    mqtt_register(&conn, &mqtt_event_handler);
    uip_ipaddr_t broker_ip;
    uip_ipaddr(&broker_ip, BROKER_IP);
    mqtt_connect(&conn, &broker_ip, BROKER_PORT, PUBLISH_INTERVAL);

    while (1) {
        mqtt_publish(&conn, topic, message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        sleep(PUBLISH_INTERVAL);
    }
    return 0;
}