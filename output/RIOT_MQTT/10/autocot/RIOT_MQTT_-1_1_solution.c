#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "MQTTPacket.h"
#include "MQTTSerializePublish.h"
#include "MQTTSubscribe.h"
#include "MQTTUnsubscribe.h"
#include "Timer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32"

MQTTClient client;
Network n;
unsigned char sendbuf[64], readbuf[64];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    MQTTPublish(&client, "device/state", &message);
    printf("Published state: work\n");
}

int main(void) {
    NetworkInit(&n);
    MQTTClientInit(&client, &n, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&n, BROKER_IP, BROKER_PORT) != 0) {
        printf("Unable to connect to broker\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }

    printf("MQTT Connected\n");

    while(1) {
        publish_state();
        sleep(5);  // Wait for 5 seconds before publishing again.
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&n);

    return 0;
}