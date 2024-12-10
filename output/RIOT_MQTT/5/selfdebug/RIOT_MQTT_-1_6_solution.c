#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <periph/gpio.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/status"
#define CLIENT_ID "esp32_client"
#define PUB_INTERVAL (5U)  // Publish every 5 seconds

static MQTTClient client;
static Network network;
static ztimer_t timer;

void publish_state(void) {
    const char *msg_payload = (rand() % 2) ? "work" : "not work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)msg_payload;
    message.payloadlen = strlen(msg_payload);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Published message: %s\n", msg_payload);
    }
}

int main(void) {
    srand(ztimer_now(ZTIMER_MSEC));  // Seed random number generator
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    ztimer_set_msg(ZTIMER_MSEC, &timer, PUB_INTERVAL * 1000U, &publish_state, thread_getpid());

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    // Initialize MQTTClient
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    // Connect to the broker
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    printf("Connected to MQTT broker\n");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);  // Sleep for the publish interval
        publish_state();
    }

    return 0;
}