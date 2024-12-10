
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdatomic.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define REMOTE_ADDR "47.102.103.1"
#define REMOTE_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work"
#define MSG_LEN (sizeof(MSG_CONTENT) - 1)

static emcute_topic_t topic;

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;
    gw.port = REMOTE_PORT;

    if (sock_udp_ep_from_str(&gw, REMOTE_ADDR) < 0) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}
