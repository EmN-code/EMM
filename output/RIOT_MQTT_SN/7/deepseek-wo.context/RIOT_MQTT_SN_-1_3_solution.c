
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"
#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    ipv4_addr_t broker_addr;

    if (ipv4_addr_from_str(&broker_addr, "47.102.103.1") == NULL) {
        puts("Error: unable to parse broker address");
        return NULL;
    }

    gw.addr.ipv4 = broker_addr;
    gw.family = AF_INET;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", STATE_MESSAGE);
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
