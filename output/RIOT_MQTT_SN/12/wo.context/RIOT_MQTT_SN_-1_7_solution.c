#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <net/ipv6/addr.h>
#include <kernel_defines.h>

#define EMCUTE_ID           "device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_LARGE];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static unsigned num_subscriptions = 0;

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    /* initialize the emcute thread */
    emcute_run(1883, EMCUTE_ID);
    return NULL;
}

int main(void) {
    /* setup our MQTT-SN broker address and port */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    /* initialize our subscriptions & topics */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    topic.name = "status";
    topic.id = 0;

    /* initialize the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker.");
        return 1;
    }

    while (1) {
        /* publish a message to the broker */
        const char *status_msg = "work";
        if (emcute_pub(&topic, status_msg, strlen(status_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish the message.");
        }

        /* sleep for 5 seconds */
        xtimer_sleep(5);
    }

    /* should be never reached */
    return 0;
}