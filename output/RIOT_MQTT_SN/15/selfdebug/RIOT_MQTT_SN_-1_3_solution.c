#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include "emcute.h"

#define EMCUTE_PORT        1883
#define PUB_TOPIC_NAME     "state"
#define PUB_INTERVAL       (5U * US_PER_SEC)

static emcute_topic_t t;
static char topic[] = PUB_TOPIC_NAME;

static void *pub_state(void *arg) {
    (void)arg;
    char message[12];

    while (1) {
        strcpy(message, "not work"); // Simulate state
        printf("Publishing: %s\n", message);
        if (emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        xtimer_sleep(PUB_INTERVAL/US_PER_SEC); // Sleep for the defined interval
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    t.name = topic;
    t.id = 0; // Will be assigned by the gateway

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    pub_state(NULL);

    emcute_discon();
    return 0;
}