#include <string.h>
#include <stdio.h>
#include "MQTTClient.h"
#include "thread.h"
#include "periph/uart.h"

#define MQTT_BROKER_ADDRESS   "47.102.103.1"
#define MQTT_BROKER_PORT      1883
#define MQTT_CLIENT_ID        "MQTT_Client_1"
#define MQTT_PUB_TOPIC        "device/status"
#define SLEEP_INTERVAL_MS     5000

static char stack[THREAD_STACKSIZE_MAIN];

int publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    return MQTTPublish(client, MQTT_PUB_TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc; // Return code

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error: Network connect failed!\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 5000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error: MQTT connection failed!\n");
        return NULL;
    }

    while (1) {
        rc = publish_state(&client, "work");
        if (rc != SUCCESS) {
            printf("Error: MQTT publish failed!\n");
        }
        xtimer_sleep(SLEEP_INTERVAL_MS / 1000);
    }
    return NULL;
}

int main(void) {
    puts("Starting MQTT example...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt thread");
    return 0;
}