#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define PUB_INTERVAL (5 * US_PER_SEC)  // Publish every 5 seconds
#define TOPIC "device/state"

static const char *mqtt_states[2] = {"work", "not work"};

void publish_state(MQTTClient *client, int state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)mqtt_states[state];
    message.payloadlen = strlen(mqtt_states[state]);
    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize the network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc;
    char address[100];
    sprintf(address, "tcp://%s:%d", MQTT_BROKER_IP, MQTT_PORT);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network, error %d\n", rc);
        return rc;
    }

    // Set up MQTT connection parameters
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker, error %d\n", rc);
        return rc;
    }

    printf("Connected to MQTT broker\n");

    int state = 0;

    while (1) {
        publish_state(&client, state);
        state = (state + 1) % 2;  // Toggle state
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
