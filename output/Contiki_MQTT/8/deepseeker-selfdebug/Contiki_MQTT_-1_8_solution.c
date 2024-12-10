
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/mqtt.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define TOPIC "state"
#define MESSAGE "work"

static struct mqtt_connection conn;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECT) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("Disconnected from MQTT broker\n");
    }
}

int main(void) {
    mqtt_status_t status;
    struct mqtt_prop_list prop_list;

    memset(&conn, 0, sizeof(conn));
    memset(&prop_list, 0, sizeof(prop_list));

    status = mqtt_register(&conn, NULL, CLIENT_ID, mqtt_event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return -1;
    }

    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, &prop_list);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        mqtt_publish(&conn, TOPIC, MESSAGE, strlen(MESSAGE), MQTT_QOS_0, false, &prop_list);
        xtimer_sleep(5);
    }

    mqtt_disconnect(&conn, &prop_list);
    return 0;
}
