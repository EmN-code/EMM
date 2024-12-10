
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;

static void _publish_state(void)
{
    const char *msg = MSG_CONTENT;
    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4) <= 0) {
        printf("Error converting address\n");
        return 1;
    }

    gw.family = AF_INET;

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
