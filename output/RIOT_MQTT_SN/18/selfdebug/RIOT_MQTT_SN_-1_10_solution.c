#include <ztimer/ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS          (1U)
#define TOPIC_MAXLEN       (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i] : %s\n",
           topic->name, (int)topic->id, in);
}

int main(void)
{
    /* Setup socket address */
    sock_udp_ep_t gw = { .port = 1883, .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Initialize emcute */
    emcute_topic_t topic;
    strcpy(topic.name, "status");

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    /* run publish-loop */
    while (1) {
        const char *pub_data = "work";
        if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}