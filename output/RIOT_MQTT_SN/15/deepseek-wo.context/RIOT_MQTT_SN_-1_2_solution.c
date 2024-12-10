
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define EMCUTE_TOPIC "state"
#define EMCUTE_MSG "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void _publish_state(void)
{
    char msg[10];
    strncpy(msg, EMCUTE_MSG, sizeof(msg));
    msg[sizeof(msg) - 1] = '\0';

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Published state message");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_ADDR, &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = EMCUTE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    puts("Topic registered");

    while (1) {
        _publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
