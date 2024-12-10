
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <dht.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <mma8x5x.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void *publish_thread(void *arg)
{
    (void)arg;
    char msg[] = "work or not work";

    while (1) {
        if (emcute_pub(&topic, msg, sizeof(msg) - 1, 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = 12345;

    if (sock_udp_create(&gw, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    gw.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_thread, NULL, "publish_thread");

    return 0;
}
