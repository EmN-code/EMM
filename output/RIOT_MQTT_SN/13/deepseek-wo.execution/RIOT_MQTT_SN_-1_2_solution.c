
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_topic_t topic;

void publish_state(void) {
    const char *msg = MESSAGE;
    size_t msg_len = strlen(msg);
    unsigned flags = 0;

    int res = emcute_pub(&topic, msg, msg_len, flags);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published message: %s\n", msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

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

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
