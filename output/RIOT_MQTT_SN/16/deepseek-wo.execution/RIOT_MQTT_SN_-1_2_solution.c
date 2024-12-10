
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define TOPIC_NAME "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

void publish_state(void) {
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

int main(void) {
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = htons(BROKER_PORT),
    };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, BROKER_ADDR) == NULL) {
        printf("Error parsing broker address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    if (emcute_init() < 0) {
        printf("Error initializing EMCUTE\n");
        return 1;
    }

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        printf("Error registering topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
