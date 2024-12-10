#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/emcute.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STATE "work"
#define INTERVAL (5U) // Publish every 5 seconds

static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv6);  // Use IPv6 family for address translation

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("[error] unable to connect: %d\n", res);
        return 1;
    }

    topic.name = "device/status";
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("[error] unable to register topic: %d\n", res);
        return 1;
    }

    while (1) {
        res = emcute_pub(&topic, STATE, strlen(STATE), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[error] unable to publish data: %d\n", res);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}