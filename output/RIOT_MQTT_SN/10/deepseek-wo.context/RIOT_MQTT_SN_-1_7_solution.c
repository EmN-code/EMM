
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>  // Include this header for inet_pton

#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static char stack[MQTT_THREAD_STACKSIZE];
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

void publish_state(void) {
    emcute_topic_t topic;
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    topic.id = 0;  // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    res = emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", MESSAGE);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        printf("Error: unable to initialize network stack\n");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error: invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Create a thread for periodic publishing
    thread_create(stack, sizeof(stack), MQTT_THREAD_PRIORITY, 0,
                  (thread_task_func_t)publish_state, NULL, "mqtt_publisher");

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(5);  // Sleep for 5 seconds
        publish_state();
    }

    return 0;
}
