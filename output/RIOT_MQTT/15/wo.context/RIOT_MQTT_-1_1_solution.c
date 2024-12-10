#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_BUFFER_SIZE 128
#define MQTT_SLEEP_SEC   5

static char mqtt_send_buf[MQTT_BUFFER_SIZE];
static char mqtt_read_buf[MQTT_BUFFER_SIZE];

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 1000, mqtt_send_buf, MQTT_BUFFER_SIZE, mqtt_read_buf, MQTT_BUFFER_SIZE);
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot-esp32";

    // Connect to the network
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc!=0) {
            printf("Network connection failed. Retrying in 1 second.\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Connect to MQTT Broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying in 1 second.\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    // Publishing state every 5 seconds
    MQTTMessage message;
    char payload[20];
    while (1) {
        snprintf(payload, sizeof(payload), "work or not work");
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, "esp32/state", &message);
        if (rc != 0) {
            printf("MQTT publish failed.\n");
        } else {
            printf("State published: %s\n", payload);
        }

        xtimer_sleep(MQTT_SLEEP_SEC);
    }

    // Disconnect
    NetworkDisconnect(&network);
    MQTTDisconnect(&client);

    return 0;
}
