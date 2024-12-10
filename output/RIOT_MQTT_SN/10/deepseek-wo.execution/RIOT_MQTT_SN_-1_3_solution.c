
#include <net/ipv6/addr.h>
#include <thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_topic_t topic;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_state, NULL, "publisher");

    return 0;
}
