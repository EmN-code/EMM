#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "RIOT_ESP32_Client"
#define TOPIC       "device/state"
#define QOS         1
#define TIMEOUT     10000L

static MQTTClient client;
static Network network;

void publish_state(void) {
    int rc;
    MQTTMessage message;
    char payload[20];

    /* Simulate a state: work or not work */
    int work_state = rand() % 2; // Randomize the state for example purposes
    strcpy(payload, (work_state) ? "work" : "not work");
    
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    
    rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Published message: %s\n", payload);
    }
}

int main(void) {
    int rc;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);
    NetworkInit(&network);
    if ((rc = NetworkConnect(&network, "47.102.103.1", 1883)) != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker at %s\n", ADDRESS);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}