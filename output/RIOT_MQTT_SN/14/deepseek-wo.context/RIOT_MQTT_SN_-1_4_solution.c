
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;

static void _publish_state(void)
{
    static int state = 0;
    const char *msg = (state++ % 2) ? MSG_NOT_WORK : MSG_WORK;
    printf("Publishing: %s\n", msg);
    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", "47.102.103.1", 1883);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect");
        return 1;
    }
    puts("Connected");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
