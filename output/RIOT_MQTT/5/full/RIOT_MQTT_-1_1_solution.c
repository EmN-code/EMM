#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread.h>

#include "MQTTClient.h"
#include "MQTTNetwork.h"
#include "esp_wifi.h"

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define CLIENT_ID "ESP32_RIOT_Client"
#define PUBLISH_TOPIC "device/state"

static MQTTClient client;
static char sendbuf[100];
static char readbuf[100];
static Network network;

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (char *) "state: working";
    message.payloadlen = strlen("state: working");
    MQTTPublish(&client, PUBLISH_TOPIC, &message);
}

void *mqtt_worker(void *arg) {
    while (1) {
        MQTTYield(&client, 1000);
        publish_state();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    int rc = 0;

    // Initialize Network
    NetworkInit(&network);

    // Connect to Network
    rc = NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);
    if (rc != 0) {
        printf("Network connect failed!\n");
        return -1;
    }

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT Broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT broker connect failed!\n");
        return -1;
    }

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, mqtt_worker, NULL, "mqtt_worker");

    thread_sleep();  // Sleep forever, letting the mqtt_worker thread handle publish

    return 0;
}