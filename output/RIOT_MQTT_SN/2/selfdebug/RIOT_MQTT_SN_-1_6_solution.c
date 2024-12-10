#include "ztimer.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static size_t num_subscriptions = 0;

static void on_publish(const emcute_topic_t *topic, void *data, size_t len)
{
    char *msg = (char *)data;
    printf("### Topic: %s ###\n", topic->name);
    printf("%.*s\n", (int)len, msg);
}

int main(void)
{
    /* initialize emcute */
    memset(subscriptions, 0, sizeof(subscriptions));
    emcute_emcute_init(stack, sizeof(stack), EMCUTE_PRIO, queue, sizeof(queue));

    /* setup the subscription to the topic "/state" */
    subscriptions[num_subscriptions].topic.name = "/state";
    subscriptions[num_subscriptions].cb = on_publish;
    emcute_sub(&subscriptions[num_subscriptions], EMCUTE_QOS_0);
    num_subscriptions++;

    /* connect to the MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    if(inet_pton(AF_INET6, "47.102.103.1", &gw.addr) == 0) {
        puts("ERROR: unable to parse destination address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to gateway");
        return 1;
    }

    /* publish state every five seconds */
    while (1) {
        char message[] = "work"; /* change this to "not work" as needed */
        emcute_topic_t t;
        unsigned flags = EMCUTE_QOS_0;

        t.name = "/state";
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("error: unable to obtain topic ID");
            return 1;
        }

        if (emcute_pub(&t, message, sizeof(message), flags) != EMCUTE_OK) {
            printf("error: unable to publish data to topic '%s'\n", t.name);
            return 1;
        }

        printf("Published %s\n", message);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    
    return 0;
}