#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define BUFFER_SIZE 128
#define PUB_TOPIC "esp32/status"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

// Function to run emcute in a separate thread
void *emcute_thread_handler(void *arg) {
    emcute_run(MQTT_BROKER_PORT, "esp32_client");
    return NULL;
}

// Connect to the MQTT-SN broker
static int connect_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };
    
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }
    
    topic.name = PUB_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    puts("Successfully connected to broker");
    return 0;
}

// Publish MQTT-SN message
static void publish_message(const char *message) {
    if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish");
    }
    else {
        printf("Published message: %s\n", message);
    }
}

int main(void) {
    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread_handler, NULL, "emcute");

    // Connect to the broker
    if (connect_broker() != 0) {
        return 1;
    }

    char *messages[2] = {"work", "not work"};
    size_t i = 0;
    
    // Periodically publish messages
    while (1) {
        publish_message(messages[i]);
        i = (i + 1) % 2;
        xtimer_usleep(INTERVAL);
    }

    return 0;
}