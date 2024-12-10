#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/inet_pton.h>
#include "xtimer.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_sub_t subscription;
static emcute_topic_t topic;
static char topic_name[] = "state";

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)len;
    char *msg = (char *)data;
    printf("Received publication: %s\n", msg);
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Couldn't connect to broker.");
        return 1;
    }
    puts("Connected to broker.");
    return 0;
}

static void publish_state(bool state) {
    char *status = state ? "work" : "not work";

    if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish.");
    }
    else {
        printf("Published: %s\n", status);
    }
}

int main(void) {
    emcute_run(SERVER_PORT, "esp32_client");

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return 1;
    }

    subscription.topic.name = topic_name;
    subscription.cb = on_pub;

    if (emcute_sub(&subscription, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe topic.");
        return 1;
    }

    if (connect_to_broker() != 0) {
        return 1;
    }

    bool state = true;
    while (1) {
        publish_state(state);
        state = !state;
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}