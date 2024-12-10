#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "shell.h"
#include <stdio.h>

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "esp32");
    return NULL;
}

int main(void) {
    /* Start emcute thread */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Specify the remote address for the MQTT-SN broker */
    sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1");

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", "47.102.103.1", EMCUTE_PORT);
        return 1;
    }

    /* Main publishing loop */
    while (1) {
        /* Create the topic */
        emcute_topic_t t;
        char* topic_name = "/status";
        t.name = topic_name;
        emcute_reg(&t);

        /* Define the state message */
        const char *state_message = "work";

        /* Publish the message */
        if (emcute_pub(&t, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }

        /* Wait for next publication */
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    /* Disconnect before exiting */
    if (emcute_discon() != EMCUTE_OK) {
        puts("error: not connected to a broker\n");
        return 1;
    }

    return 0;
}