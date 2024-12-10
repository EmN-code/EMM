#include "net/sock/util.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "assert.h"
#include "mutex.h"
#include "dht_params.h"
#include "xtimer.h"
#include "time_units.h"
#include "dht.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "thread.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COMMAND_TIMEOUT_MS (4000)
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define STACK_SIZE  (THREAD_STACKSIZE_DEFAULT)
#define BUFFER_SIZE (128)

// Buffers for MQTT Client
unsigned char sendbuf[BUFFER_SIZE];
unsigned char readbuf[BUFFER_SIZE];

// Thread Stack memory
char stack[STACK_SIZE];

// Placeholder for threading and MQTT (ensure stack is defined but ignored comment for now)
kernel_pid_t thread_pid;

void *mqtt_publisher(void *arg) {
    Network network;
    MQTTClient client;

    // Initialize Network
    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    if (rc != 0) {
        printf("Network connection failed: %d\n", rc);
        return NULL;
    }

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_esp32";

    rc = MQTTConnect(&client, &connectData);

    if (rc != 0) {
        printf("MQTT connection failed: %d\n", rc);
        return NULL;
    }

    while (1) {
        // Create MQTT Message
        MQTTMessage msg;
        msg.qos = QOS0;
        msg.retained = 0;
        msg.payload = (void *)"work";
        msg.payloadlen = strlen("work");

        // Publish message
        if ((rc = MQTTPublish(&client, "state", &msg)) != 0) {
            printf("MQTT publish failed: %d\n", rc);
        } else {
            printf("Message published: work\n");
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // Start MQTT Publisher Thread
    thread_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_publisher, NULL, "mqtt_publisher");
    if (thread_pid < 0) {
        printf("Thread creation failed\n");
        return 1;
    }
    return 0;
}