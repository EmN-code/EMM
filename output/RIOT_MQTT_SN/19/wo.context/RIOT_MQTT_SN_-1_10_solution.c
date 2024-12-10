#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_TOPIC      "state"
#define MQTT_MESSAGE    "work"
#define PUB_INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) {
        if (emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Publish failed");
        }
        else {
            puts("Data published");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Connection failed");
        return 1;
    }
    puts("Connected to gateway");

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Topic registration failed");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    return 0;
}