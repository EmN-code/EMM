
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT       1883
#define EMCUTE_TOPIC      "state"
#define EMCUTE_BUFSIZE    64
#define EMCUTE_KEEPALIVE  60
#define EMCUTE_QOS        0

static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;
    char buf[EMCUTE_BUFSIZE];

    snprintf(buf, sizeof(buf), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, buf, strlen(buf), EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", buf);
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    topic.name = EMCUTE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
