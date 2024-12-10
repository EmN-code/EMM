#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"
#include "net/af.h"
#include "net/protnet/ipaddr.h"
#include "net/sock/udp"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32Client"
#define TOPIC "state"
#define PUBLISH_INTERVAL 5

void publish_state(MQTTClient* client) {
    const char* state = "work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);
    MQTTPublish(client, TOPIC, &message);
    printf("Published: %s\n", state);
}

int main(void) {
    Network network;
    MQTTClient client;
    Timer timer;

    char sendbuf[100], readbuf[100];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTStartTask(&client);

    MQTTConnectData data = MQTTConnectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENT_ID;
    data.username.cstring = NULL;
    data.password.cstring = NULL;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }
    printf("MQTT connected\n");

    while (1) {
        publish_state(&client);
        TimerCountdown(&timer, PUBLISH_INTERVAL);
        while (!TimerIsExpired(&timer)) {
            MQTTYield(&client, 100);
        }
    }

    return 0;
}