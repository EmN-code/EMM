#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt_client.h"
#include "network.h"
#include "timer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static volatile int state = 0;  // 0: not work, 1: work

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    char message_content[20];

    snprintf(message_content, 20, "%s", state ? "work" : "not work");
    message.payload = (void *)message_content;
    message.payloadlen = strlen(message_content);
    message.qos = QOS0;
    message.retained = 0;

    MQTTPublish(client, "state", &message);
}

void timer_callback(void *arg) {
    MQTTClient *client = (MQTTClient *)arg;
    publish_state(client);
    state = !state; // Toggle state for testing
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connection failed\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }

    printf("MQTT connected\n");

    Timer timer;
    TimerInit(&timer);
    TimerCountdownMS(&timer, 5000); // 5 seconds interval

    while (1) {
        if (TimerIsExpired(&timer)) {
            timer_callback(&client);
            TimerCountdownMS(&timer, 5000);
        }
    }

    return 0;
}