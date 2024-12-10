#include <ztimer.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include "thread.h"

#define MQTT_SN_PORT 1883
#define SERVER_IP "47.102.103.1"
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5U) // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static int publish_state(void) {
    // Define the topic
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    
    t.name = TOPIC_NAME;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    const char *data = "work";
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }

    puts("Data published");
    return 0;
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(MQTT_SN_PORT, CLIENT_ID);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return NULL;
    }

    puts("Connected to gateway");

    while (1) {
        publish_state();
        sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    /* Run indefinitely */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}