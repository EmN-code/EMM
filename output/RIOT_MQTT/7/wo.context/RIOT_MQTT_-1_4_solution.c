#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MSG "work"
#define MQTT_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static Network network;
static MQTTClient client;
static unsigned char sendbuf[80], readbuf[80];

static void mqtt_publish_thread(void *arg) {
    (void)arg;

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);

    while (1) {
        printf("Publishing message: %s on topic: %s\n", MQTT_MSG, MQTT_TOPIC);
        MQTTPublish(&client, MQTT_TOPIC, &message);
        xtimer_usleep(MQTT_INTERVAL);
    }
}

int main(void) {
    int res;

    printf("Setting up network...\n");
    NetworkInit(&network);

    printf("Initializing MQTT client...\n");
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to MQTT broker at %s:%d...\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    res = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (res != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_device";

    res = MQTTConnect(&client, &connect_data);
    if (res != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("MQTT client connected\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL, "mqtt_publisher");

    return 0;
}