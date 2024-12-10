#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "MQTTClient.h" // Ensure this is the last header to resolve dependencies

#define MQTT_CLIENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_CLIENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state/topic"

static char mqtt_client_stack[MQTT_CLIENT_THREAD_STACKSIZE];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(void) {
    static const char* message = "work"; // Example state to publish
    MQTTMessage mqMsg;
    mqMsg.payload = (void*)message;
    mqMsg.payloadlen = strlen(message);
    mqMsg.qos = QOS0;
    mqMsg.retained = 0;

    if (MQTTPublish(&client, MQTT_TOPIC, &mqMsg) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published successfully\n");
    }
}

void *mqtt_client_thread(void *arg) {
    (void) arg;
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 5000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32-client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to broker\n");
        return NULL;
    }
    printf("Connected to broker\n");

    while (1) {
        publish_state();
        sleep(5); // Delay for 5 seconds before publishing again
    }

    return NULL;
}

int main(void) {
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack),
                  MQTT_CLIENT_THREAD_PRIORITY, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");
    return 0;
}