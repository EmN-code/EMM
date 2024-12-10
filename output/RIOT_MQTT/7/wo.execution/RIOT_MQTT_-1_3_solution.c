#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

// Function to initialize the network
int init_network(Network *n) {
    return NetworkConnect(n, BROKER_IP, BROKER_PORT);
}

// Function to publish a message
void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, "state", &message);
}

int main(void) {
    // Initialize network
    if (init_network(&network) != 0) {
        printf("Failed to initialize the network.\n");
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    // Connect to the MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to the broker.\n");
        return -1;
    }

    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_MSEC, MQTT_PUBLISH_INTERVAL / 1000);
    }

    return 0;
}