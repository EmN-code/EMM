#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define IP_ADDR_SIZE    (16U)
#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     (1883U)
#define PUBLISH_INTERVAL (5U * 1000000U)
#define TOPIC_NAME      "state"
#define TOPIC_DATA      "work"

static emcute_topic_t my_topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

int main(void) {
    ipv6_addr_t server_addr;

    // Convert IPv4 to IPv6 mapped IPv4 address
    char ipv4_mapped_addr_str[IP_ADDR_SIZE] = "::FFFF:47.102.103.1";

    if (ipv6_addr_from_str(&server_addr, ipv4_mapped_addr_str) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }
    memcpy(&gw.addr.ipv6, &server_addr, sizeof(server_addr));

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    my_topic.name = TOPIC_NAME;
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&my_topic, TOPIC_DATA, sizeof(TOPIC_DATA), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}