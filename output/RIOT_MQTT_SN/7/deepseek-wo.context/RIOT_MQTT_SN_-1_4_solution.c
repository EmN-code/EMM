
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define EMCUTE_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT        (1883)
#define EMCUTE_ADDR        "47.102.103.1"
#define TOPIC_NAME         "state"
#define PUBLISH_INTERVAL   (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[EMCUTE_STACKSIZE];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IP address");
        return NULL;
    }

    gw.family = AF_INET;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    puts("Topic registered");

    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published state: work");
        }

        xtimer_usleep(PUBLISH_INTERVAL);

        // Toggle state
        if (strcmp(state, "work") == 0) {
            strcpy(state, "not work");
        } else {
            strcpy(state, "work");
        }
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN example");

    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
