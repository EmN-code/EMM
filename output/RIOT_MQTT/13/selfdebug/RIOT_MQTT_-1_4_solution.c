#include <stdio.h>
#include "net/mqtt.h"
#include "net/network_layer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define BUFFER_SIZE 128
#define MQTT_TOPIC "device/state"

static char* states[2] = {"work", "not work"};

void on_pub_completion(void* arg, int status) {
    (void)arg;
    if (status == MQTT_SUCCESS) {
        printf("Publish successful\n");
    } else {
        printf("Publish failed\n");
    }
}

int main(void) {
    char buffer[BUFFER_SIZE];
    MQTTClient client;
    Network network;

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, buffer, BUFFER_SIZE, buffer, BUFFER_SIZE);

    // Connect to the broker
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connection failed\n");
        return -1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }

    printf("MQTT connected\n");
    int state_idx = 0;

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = states[state_idx];
        message.payloadlen = strlen(states[state_idx]);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Error while publishing message\n");
        }

        state_idx = 1 - state_idx; // Toggle state
        thrd_sleep(5);
    }

    return 0;
}