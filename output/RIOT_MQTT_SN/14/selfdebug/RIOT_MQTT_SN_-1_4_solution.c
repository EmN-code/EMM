#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U) // publish every 5 seconds
#define MESSAGE "work"

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Published %.*s
", (int)len, (char *)data);
}

int main(void)
{
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };

    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: unable to parse broker IP");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, MESSAGE, strlen(MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
            return 1;
        }

        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return 0;
}