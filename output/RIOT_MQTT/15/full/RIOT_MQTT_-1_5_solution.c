#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_system.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_STACK_SIZE 1024U
#define MQTT_PUBLISH_INTERVAL 5

static char mqtt_client_stack[MQTT_CLIENT_STACK_SIZE];
static MQTTClient client;
static Network network;

void mqtt_publish_task(void *arg) {
    (void)arg; // Suppress unused parameter warning
    int rc;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work or not work";
    message.payloadlen = strlen((char *)message.payload);
    
    while (1) {
        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            // Handle publish error
            printf("Error publishing message: %d\n", rc);
        }
        // Wait for the specified publish interval
        ztimer_sleep(ZTIMER_MSEC, MQTT_PUBLISH_INTERVAL * MS_PER_SEC);
    }
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the network
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            // Network connection failed, retry
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up connection parameters
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    // Connect to the broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            // MQTT connection failed, retry
            printf("MQTT connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }

    // Start a new thread for publishing messages
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_task, NULL, "mqtt_publish_task");

    return 0;
}