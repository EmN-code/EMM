#include <stdlib.h>
#include <thread.h>
#include <stdio.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_CLIENT_ID "ESP32_Client"
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "pass"

static char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* mqtt_thread(void *arg) {
    /* Create an MQTT client */
    MQTTClient client;
    Network network;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    /* Connect to the MQTT broker */
    char address[100];
    sprintf(address, "%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    printf("Connecting to MQTT broker at %s...\n", address);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code: %d\n", rc);
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.username.cstring = MQTT_USERNAME;
    connectData.password.cstring = MQTT_PASSWORD;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return NULL;
    }

    printf("Successfully connected to MQTT broker\n");

    /* Publish messages every 5 seconds */
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;

    while (1) {
        char payload[20];
        sprintf(payload, "work");
        message.payload = payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code: %d\n", rc);
        }
        else {
            printf("Message published: %s\n", payload);
        }

        /* Sleep for 5 seconds */
        sleep(5);
    }

    return NULL;
}

int main(void) {
    /* Start the MQTT thread */
    printf("Starting MQTT client thread\n");
    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    /* Main thread continues running, possibly handling other tasks */
    while (1) {
        sleep(1);
    }

    return 0;
}