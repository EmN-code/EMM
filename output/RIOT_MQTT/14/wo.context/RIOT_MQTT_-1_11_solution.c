#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "net/sock/udp.h"
#include "msg.h"
#include "timex.h"
#include "ztimer.h"
#include "net/paho_mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC   "device/state"
#define PUB_MESSAGE "work"
#define BUFFER_SIZE 100

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    int result;
    char buf[BUFFER_SIZE];
    
    // Prepare the message
    message.qos = QOS1;
    message.retained = 0;
    message.payload = PUB_MESSAGE;
    message.payloadlen = strlen(PUB_MESSAGE);

    // Publish the message
    result = MQTTPublish(&client, PUB_TOPIC, &message);
    if (result < 0) {
        printf("MQTTPublish failed with result code: %d\n", result);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    int result;
    unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);
    
    result = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (result != 0) {
        printf("NetworkConnect failed with result code: %d\n", result);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "mqtt_client";

    result = MQTTConnect(&client, &connectData);
    if (result != 0) {
        printf("MQTTConnect failed with result code: %d\n", result);
        return -1;
    }

    printf("MQTT Connected\n");

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000); // 5 second delay
    }

    return 0;
}