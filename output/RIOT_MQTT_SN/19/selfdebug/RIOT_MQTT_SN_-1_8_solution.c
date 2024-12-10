#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <net/emcute.h>
#include <xtimer.h>

#define EMCUTE_ID           "state_publisher"
#define EMCUTE_PORT         1883
#define BROKER_IPv6         "47.102.103.1"
#define STATE_TOPIC         "device/state"
#define INTERVAL            (5U * US_PER_SEC)  // 5 seconds

static emcute_topic_t state_topic;

static void publisher(void)
{
    char *state_message = "work";
    while (1) {
        int res = emcute_reg(&state_topic);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to register the topic: %d\n", res);
            continue;
        }

        res = emcute_pub(&state_topic, state_message, strlen(state_message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish the message: %d\n", res);
        }

        xtimer_sleep(INTERVAL);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET6, BROKER_IPv6, &gw.addr) != 1) {
        printf("Error parsing broker address\n");
        return 1;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker: %d\n", res);
        return 1;
    }

    state_topic.name = STATE_TOPIC;

    publisher();  // Start the publisher loop

    return 0;
}
