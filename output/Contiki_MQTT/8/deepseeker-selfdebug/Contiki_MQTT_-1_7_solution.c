
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "pass"
#define MQTT_MESSAGE "work"

static struct mqtt_connection conn;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("MQTT disconnected\n");
            break;
        default:
            break;
    }
}

int main(void) {
    // Initialize the network stack
    netstack_init();

    // Initialize the MQTT connection
    mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_callback, 0);

    // Set username and password
    mqtt_set_username_password(&conn, MQTT_USERNAME, MQTT_PASSWORD);

    // Connect to the MQTT broker
    mqtt_status_t status = mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS_0, 0, 0, MQTT_TOPIC_ALIAS_EN_NO, NULL);
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
