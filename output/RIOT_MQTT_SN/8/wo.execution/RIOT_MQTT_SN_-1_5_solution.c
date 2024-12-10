#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "mma8x5x.h"
#include "stdio.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "stdlib.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5U)  // Interval in seconds

static char client_id[] = "esp32-client";
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    thread_sleep();  // Sleep indefinitely for the emcute thread to run
    return NULL;
}

int main(void) {
    // Setup a network endpoint
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr);

    // Start emcute thread for receiving messages
    char emcute_stack[THREAD_STACKSIZE_MAIN];
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN, 0, emcute_thread, NULL, "emcute");

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker.");
        return 1;
    }

    // Register the topic
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic.");
        return 1;
    }

    while (1) {
        // Publish the message
        const char *msg = "work";
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message.");
            return 1;
        }
        thread_sleep(MSG_INTERVAL);  // Replace ztimer_sleep with thread_sleep if necessary
    }

    return 0;  // Should never be reached
}