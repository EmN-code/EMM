#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/mqtt.h"
#include "net/sock/udp.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MESSAGE "work"

static int _publish(mqtt_t *client, const char *topic, const char *msg) {
    mqtt_publish(client, MQTT_TOPIC, msg, strlen(msg), MQTT_QOS_0, 0);
    return 0;
}

static int _connect(mqtt_t *client, mqtt_broker_t *broker) {
    int rc;
    char client_id[64];
    snprintf(client_id, sizeof(client_id), "esp32_client_%i", rand());

    while (1) {
        rc = mqtt_connect(client, broker, false, MQTT_QOS_0, NULL, NULL, NULL, 0, false, client_id);
        if (rc == MQTT_OK) {
            printf("Connected to MQTT Broker: %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            break;
        }
        else {
            printf("Connection failed, retrying...\n");
            xtimer_sleep(1);
        }
    }
    return rc;
}

int main(void) {
    mqtt_client_t client;
    mqtt_broker_t broker;

    broker.hostname = MQTT_BROKER_IP;
    broker.port = MQTT_BROKER_PORT;

    _connect(&client, &broker);

    while (1) {
        _publish(&client, MQTT_TOPIC, MESSAGE);
        printf("Published: %s\n", MESSAGE);
        xtimer_sleep(5);
    }
    return 0;
}