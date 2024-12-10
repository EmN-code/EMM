
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_t sock;

void publish_state(emcute_topic_t *topic, const char *state) {
    int res = emcute_pub(topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %s\n", state);
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDR, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        publish_state(&topic, MSG_WORK);
        xtimer_usleep(INTERVAL);
        publish_state(&topic, MSG_NOT_WORK);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
