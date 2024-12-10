#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char emcute_topic_name[NUMOFSUBS][TOPIC_MAXLEN];

static const sock_udp_ep_t server = { .family = AF_INET6, .port = 1883 };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(1883, "my_riot_client");
    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example application");

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(emcute_topic_name, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to the broker */
    if (emcute_con((sock_udp_ep_t *)&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    /* setup topic for publishing */
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    strcpy(emcute_topic_name[0], "device/state");
    topic.name = emcute_topic_name[0];

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    /* start publishing the state every 5 seconds */
    pkg_t data;
    memset(&data, 0, sizeof(data));
    strcpy((char *)data.buf, "work or not work");
    data.len = strlen((char *)data.buf);

    while (1) {
        if (emcute_pub(&topic, data.buf, data.len, EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        xtimer_sleep(5);
    }

    return 0;
}