#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PORT         (1883U)
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         (1883U)
#define TOPIC_NAME          "state"
#define PUB_INTERVAL        (5U * 1000000U)

static char _stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t _topic;

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("error: unable to parse server address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: not able to connect to broker\n");
        return 1;
    }
    return 0;
}

static int register_topic(void) {
    unsigned flags = EMCUTE_QOS_0;
    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }
    return 0;
}

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&_topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }
    puts("published successfully");
    return 0;
}

static void *thread_handler(void *arg) {
    (void)arg;
    char *state = "work";
    while (1) {
        publish_state(state);
        xtimer_usleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, ARRAY_SIZE(_msg_queue));
    if (connect_to_broker() != 0) {
        return 1;
    }
    if (register_topic() != 0) {
        return 1;
    }
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "pub_thread");
    return 0;
}