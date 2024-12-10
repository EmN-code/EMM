#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/sock/tcp.h>
#include <kernel_defines.h>
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883

Network network;
MQTTClient client;
int rc = 0;

void publish_work_state() {
    static int work_state = 0;
    const char* state_message = work_state ? "work" : "not work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = false;
    message.payload = (void*)state_message;
    message.payloadlen = strlen(state_message);

    if ((rc = MQTTPublish(&client, "state", &message)) != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    }
    work_state = !work_state;  // Toggle work state
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, ADDRESS, PORT)) != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;
    
    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT Broker: %d\n", rc);
        return rc;
    }

    while (1) {
        publish_work_state();
        sleep(5);  // Wait for 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}