#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <assert.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define STACK_SIZE  (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL  (5U * US_PER_SEC)  // 5 seconds

static char stack[STACK_SIZE];
static MQTTClient client;
static Network network;
static volatile bool running = true;

// Task function to publish data to the MQTT broker
tatic void *publish_state(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Device";

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connection failed\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connection failed\n");
        return NULL;
    }

    while (running) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        const char *payload = "work";
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error while publishing state\n");
        } else {
            printf("State published successfully\n");
        }

        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, publish_state, NULL, "mqtt_pub_thread");
    return 0;
}