#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include "paho_mqtt.h"
#include "MQTTClient.h"  // Ensure this is the last inclusion

#define MQTT_PUBLISH_INTERVAL     (5U * US_PER_SEC)
#define MQTT_BROKER_IP            "47.102.103.1"
#define MQTT_BROKER_PORT          1883
#define MQTT_TOPIC                "esp32/status"

static MQTTClient client;
static Network network;

void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;  // Quality of Service level 0
    message.retained = 0;  // Non-retained message
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Error: MQTT publish failed!\n");
    }
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error: Network connection failed!\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error: MQTT connection failed!\n");
        return -1;
    }

    char *states[] = {"work", "not work"};
    int state_index = 0;

    while (1) {
        publish_state(states[state_index]);
        state_index = (state_index + 1) % 2;
        xtimer_sleep(MQTT_PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}