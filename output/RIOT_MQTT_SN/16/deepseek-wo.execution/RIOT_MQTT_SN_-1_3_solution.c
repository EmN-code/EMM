
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define TOPIC_NAME "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;

void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    size_t len = strlen(state);

    int res = emcute_pub(&topic, state, len, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("Published state: %s\n", state);
    }

    state_index = (state_index + 1) % (sizeof(states) / sizeof(states[0]));
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = HTONS(BROKER_PORT),
    };

    if (inet_aton(BROKER_ADDR, &remote.addr.in.sin_addr) == 0) {
        printf("Invalid broker address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    emcute_init();

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
