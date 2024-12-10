#include "net/sock/udp.h"
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "emcute.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_INTERVAL      (5U * US_PER_SEC)
#define DEFAULT_PORT        1883
#define DEFAULT_SERVER      "47.102.103.1"

static emcute_topic_t state_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static char *topics[] = { "state" };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static void *emcute_thread(void *arg) {
    emcute_run(DEFAULT_PORT, EMCUTE_ID);
    return NULL;
}

static int setup_mqtt(void) {
    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* setup subscription to receive state change messages */
    emcute_sub_t subscription = { .cb = on_pub, .topic = { .name = topics[0] } };
    emcute_sub(&subscription, EMCUTE_QOS_0);
    
    /* setup connection to gateway */
    sock_udp_ep_t gw = { .family = AF_INET, .port = DEFAULT_PORT };
    inet_pton(AF_INET6, DEFAULT_SERVER, &gw.addr.ipv6);
    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

int main(void) {
    if (setup_mqtt() != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    while (1) {
        const char *state = "work";
        emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0);
        puts("state published");

        xtimer_sleep(STATE_INTERVAL / US_PER_SEC);
    }
    
    return 0;
}