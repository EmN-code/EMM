# Include necessary RIOT and MQTT Paho headers in descending order
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "net/af.h"
#include "net/protn.h"
#include "net/sock.h"

// Necessary defines and global variables
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_device"

static char stack[THREAD_STACKSIZE_DEFAULT];

// Function to publish a message
static void *mqtt_publish_loop(void *arg) {
    (void)arg;
    sock_udp_ep_t broker = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4); // Set broker IP
    broker.port = MQTT_BROKER_PORT; // Set broker port

    MQTTClient client;
    Network network;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0); // Initialize the client

    printf("Connecting to MQTT broker %s:%d...\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("[ERROR] Failed to connect to broker\n");
        return NULL;
    }

    data.MQTTVersion = 4;
    data.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("[ERROR] Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
            printf("[ERROR] Failed to publish work state\n");
        }

        printf("MQTT message published: work\n");

        xtimer_sleep(5); // Wait for 5 seconds
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_loop, NULL, "mqtt_pub");
    return 0;
}