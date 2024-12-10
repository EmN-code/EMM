#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;

static void *mqtt_publisher(void *arg) {
    (void)arg;
    MQTTMessage message;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = "Not Work";
    message.payloadlen = strlen(message.payload);

    while (1) {
        // Publish a message
        int rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("Error: MQTT publish failed with code %d\n", rc);
        } else {
            printf("Published state: Not Work\n");
        }

        // Wait before publishing again
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * MS_PER_SEC);
    }
    return NULL;
}

int main(void) {
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error: Network connection failed with code %d\n", rc);
        return 1;
    }

    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";

    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Error: MQTT connection failed with code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);

    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_publisher, NULL, "MQTT-Publisher") <= KERNEL_PID_UNDEF) {
        printf("Error: Thread creation failed\n");
        return 1;
    }

    return 0;
}