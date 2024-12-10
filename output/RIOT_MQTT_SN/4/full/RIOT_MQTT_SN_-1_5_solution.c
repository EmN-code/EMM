#include <xtimer.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <string.h>
#include <timex.h>
#include <arpa/inet.h>

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (1U)
#define TOPIC_MAXLEN    (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static emcute_topic_t t;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("### Got publication for topic %i [%.*s] ###\n",
           (int)topic->id, (int)len, in);
}

int main(void)
{
    /* initialize thread message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize emcute */
    emcute_init();

    /* connect to MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* set topic name */
    t.name = "state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        /* publish "work" or "not work" */
        const char *data = (rand() % 2 == 0) ? "work" : "not work";
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }

        xtimer_sleep(5);
    }

    /* should never be reached */
    return 0;
}