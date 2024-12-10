// Include necessary header files
#include <stdlib.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "net/ipv6/addr.h"

#define MQTT_PORT      1883
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_WORKING_TOPIC "device/status"
#define MQTT_MESSAGE   "work"

// Define stack size and priority for the MQTT thread
#define MQTT_THREAD_STACKSIZE      THREAD_STACKSIZE_LARGE
#define MQTT_THREAD_PRIORITY       (THREAD_PRIORITY_MAIN - 1)

static mqttsn_client_t mqtt_client;
static sock_udp_ep_t gw = { .family = AF_INET6 };

// Function to initialize the MQTT client
void init_mqtt(void) {
    printf("MQTT client setup\n");
    mqttsn_init(&mqtt_client);
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);
    gw.port = MQTT_PORT;

    mqttsn_register(&mqtt_client, 0xDEAD);
}

// Function to publish a message
void publish_status(void) {
    mqttsn_pub_t pub;

    pub.topic_name = MQTT_WORKING_TOPIC;
    pub.msg.data = (uint8_t *)MQTT_MESSAGE;
    pub.msg.len = sizeof(MQTT_MESSAGE);

    printf("Sending MQTT message: %s\n", MQTT_MESSAGE);
    mqttsn_publish(&mqtt_client, &pub);
}

void *mqtt_thread(void *arg) {
    (void)arg;

    while (1) {
        publish_status();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("MQTT client example\n");
    init_mqtt();

    char mqtt_stack[MQTT_THREAD_STACKSIZE];
    thread_create(mqtt_stack, sizeof(mqtt_stack), MQTT_THREAD_PRIORITY, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}

/* Makefile fragment */
#MODULE += mqtt  // Uncomment this line to include MQTT module
