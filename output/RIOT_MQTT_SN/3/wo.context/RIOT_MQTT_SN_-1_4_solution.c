#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include "emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

#define PUBLISH_INTERVAL (5LU * US_PER_SEC)

static emcute_topic_t topic;
static sock_udp_ep_t gw = {.family = AF_INET, .port = BROKER_PORT};

static int on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    puts("Published data");
    return EMCUTE_OK;
}

static void *publisher_thread(void *arg) {
    (void)arg;
    char message[20];

    while (1) {
        strcpy(message, "state work");

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    sock_udp_ep_t remote;

    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    emcute_run(BROKER_PORT, "emcute-client");

    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = "test";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    kernel_pid_t pub_thread = thread_create(pub_thread_stack, sizeof(pub_thread_stack), 3, 0, publisher_thread, NULL, "publisher");
    if (pub_thread < 0) {
        puts("Failed to create publisher thread");
        return 1;
    }

    return 0;
}