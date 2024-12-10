
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define TOPIC_NAME "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static emcute_topic_t topic;

static void _on_pub(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
}

static int _publish_state(emcute_topic_t *topic)
{
    const char *state = "work";
    unsigned flags = EMCUTE_QOS_0;

    return emcute_pub(topic, state, strlen(state), flags);
}

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;
    gw.port = BROKER_PORT;
    gw.family = AF_INET;
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    emcute_sub_t sub;
    sub.topic.name = TOPIC_NAME;
    sub.cb = _on_pub;
    sub.arg = NULL;

    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        if (_publish_state(&topic) != EMCUTE_OK) {
            puts("Error publishing state");
        }
        xtimer_sleep(5);
    }

    return 0;
}
