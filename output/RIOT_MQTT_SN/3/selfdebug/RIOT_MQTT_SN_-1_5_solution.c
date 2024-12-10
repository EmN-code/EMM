#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h> // Fix for implicit declaration of 'inet_pton'
#include <kernel_defines.h> // Include any required kernel definitions

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID_MAXLEN    (32U)
#define STATE_MSG           "work"
#define NON_STATE_MSG       "not work"

static char stack[THREAD_STACKSIZE_MAIN];

static emcute_topic_t topic;
static char tname[EMCUTE_ID_MAXLEN] = "topic";

static void *emcute_thread(void *arg) {
    (void)arg;

    while (1) {
        char *msg = ((xtimer_now() % 10) < 5) ? STATE_MSG : NON_STATE_MSG;
        emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    /* initialize the Emcute stack */
    emcute_setup(THREAD_CREATE_STACKSIZE_MAIN, "emcute", EMCUTE_PRIO);

    /* connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", "47.102.103.1", EMCUTE_PORT);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n", "47.102.103.1", EMCUTE_PORT);

    /* create a thread that publishes messages */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute-thread");

    return 0;
}