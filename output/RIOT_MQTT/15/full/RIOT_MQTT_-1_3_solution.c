# Import necessary headers
#include <arpa/inet.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <thread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <freertos/task.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"

// Global variables
static MQTTClient client;
static Network network;
unsigned char sendbuf[80], readbuf[80];

void connect_to_mqtt() {
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_ESP32";

    while (1) {
        int rc = MQTTConnect(&client, &options);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }
}

void publish_state(void *args) {
    MQTTMessage message;
    char *state = "work";

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    while (1) {
        int rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Publish failed. Retrying...\n");
        } else {
            printf("State published successfully.\n");
        }
        sleep(5);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_to_mqtt();

    // Start a new thread for publishing state every 5 seconds
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_state, NULL, "publish_state");

    // Start the FreeRTOS task scheduler
    vTaskStartScheduler();

    return 0;
}