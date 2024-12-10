
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define EMCUTE_PORT       1883
#define EMCUTE_BUFSIZE    128
#define EMCUTE_TOPIC_MAXLEN 64

#define TOPIC_NAME        "state"
#define STATE_MESSAGE     "work or not work"

static emcute_topic_t topic;
static sock_udp_t sock;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_t broker_addr;

    if (ipv6_addr_from_str(&broker_addr, "47.102.103.1") == NULL) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    memcpy(&remote.addr.ipv6, &broker_addr, sizeof(broker_addr));

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    memset(&topic, 0, sizeof(topic));
    strncpy(topic.name, TOPIC_NAME, EMCUTE_TOPIC_MAXLEN);

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return 0;
}
