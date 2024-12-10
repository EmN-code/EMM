
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MSG "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    int res;
    unsigned flags = EMCUTE_QOS_0;

    res = emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG) - 1, flags);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("State published successfully\n");
    }
}

int main(void) {
    int res;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = 12345;

    gw.family = AF_INET;
    gw.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    res = sock_udp_create(&local, NULL, &gw, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
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
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
