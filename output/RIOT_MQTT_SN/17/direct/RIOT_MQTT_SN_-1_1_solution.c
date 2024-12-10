#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/conn.h"

#define EMCUTE_ID       "esp32-node"
#define EMCUTE_PORT     1883
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define SERVER_ADDR     "47.102.103.1"
#define PUB_TOPIC       "esp32/status"
#define PUB_INTERVAL    (5U)  // 5 seconds in ticks

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[1];
static char *topics[] = {PUB_TOPIC};

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

static int publish_state(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = PUB_TOPIC;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("[Failed] Unable to register topic!");
        return 1;
    }

    const char *message = "work";  // The message to be published
    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        puts("[Failed] Unable to publish data!");
        return 1;
    }
    printf("Published `%s` on topic `%s`\n", message, t.name);

    return 0;
}

int main(void) {
    puts("RIOT MQTT-SN example application\n");

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[Failed] Unable to connect to broker.");
        return 1;
    }
    puts("Successfully connected to broker\n");

    /* periodically publish state */
    while (1) {
        xtimer_sleep(PUB_INTERVAL);
        publish_state();
    }

    return 0;
}