#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define EMCUTE_ID "esp32_client"
#define EMCUTE_PORT 1883
#define BROKER_IP "47.102.103.1"
#define PUB_TOPIC_NAME "esp32/status"

static emcute_topic_t topic;
static sock_udp_ep_t remote;

static void on_pub(void) {
    char *message = "work or not work"; // Message to publish
    printf("Publishing on topic %s: %s\n", PUB_TOPIC_NAME, message);

    if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
}

static int init_esp32(void) {
    int res;

    // Parsing the broker IP address
    res = inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);
    if (res == 0) {
        printf("Error: invalid IP address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = EMCUTE_PORT;

    // Connect to the broker
    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return -1;
    }

    // Register the topic
    strcpy(topic.name, PUB_TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return -1;
    }

    return 0;
}

int main(void) {
    // Initialize the ESP32 and connect to the broker
    if (init_esp32() != 0) {
        return 1;
    }

    // Periodic publication every 5 seconds
    while (1) {
        on_pub();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}