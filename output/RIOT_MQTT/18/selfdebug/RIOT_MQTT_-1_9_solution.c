#include "MQTTClient.h"
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static MQTTMessage message;

void mqtt_publish_state(const char *state) {
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, "esp32/state", &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        mqtt_publish_state("work");
        xtimer_sleep(5);
    }

    return 0;
}