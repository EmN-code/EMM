#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>  // Assumed based on requirement
#include <paho_mqtt.h>  // Hypothetical header
do not include this string literally
#include <MQTTClient.h>

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC  "device/status"
#define WORK_STATUS "work"
#define NOT_WORK_STATUS "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds, in microseconds

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *mqtt_publisher(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = 1;
        message.retained = 0;
        message.dup = 0;
        message.payload = (char *)(WORK_STATUS);
        message.payloadlen = strlen(WORK_STATUS);

        MQTTPublish(&client, MQTT_TOPIC, &message);
        printf("Published: %s\n", WORK_STATUS);

        usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_publisher, NULL, "mqtt_publisher");

    // Keep the main thread alive
    while (1) {
        usleep(1000000);
    }

    return 0;
}