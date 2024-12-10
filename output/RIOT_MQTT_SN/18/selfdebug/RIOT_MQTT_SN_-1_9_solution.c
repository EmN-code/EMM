#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <thread.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static emcute_topic_t state_topic;
static const char *message = "work or not work";

static int publish_state(void) {
    unsigned flags = EMCUTE_QOS_0;
    return emcute_pub(&state_topic, message, strlen(message), flags);
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        puts("Error: unable to parse IPv6 address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return NULL;
    }

    state_topic.name = "state";
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        publish_state();
        puts("State published");
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN example connection");

    /* Initialize emcute */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}