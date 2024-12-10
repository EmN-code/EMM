
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_TOPIC "state"
#define EMCUTE_CLIENT_ID "esp32_client"
#define EMCUTE_BROKER_IP "47.102.103.1"
#define EMCUTE_BROKER_PORT 1883
#define EMCUTE_QOS 0
#define EMCUTE_RETAIN 0
#define EMCUTE_BUFSIZE 64
#define EMCUTE_TOPIC_MAXLEN 32
#define EMCUTE_KEEPALIVE 60
#define EMCUTE_PING_INTERVAL (EMCUTE_KEEPALIVE / 4)

static char emcute_buf[EMCUTE_BUFSIZE];
static emcute_topic_t emcute_topic;

static void _emcute_pub(void)
{
    const char *state = (rand() % 2) ? "work" : "not work";
    int res = emcute_pub(&emcute_topic, state, strlen(state), EMCUTE_QOS | EMCUTE_RETAIN);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = EMCUTE_BROKER_PORT;

    if (inet_pton(AF_INET, EMCUTE_BROKER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error converting IP address\n");
        return 1;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    emcute_topic.name = EMCUTE_TOPIC;
    res = emcute_reg(&emcute_topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        _emcute_pub();
        xtimer_sleep(5);
    }

    return 0;
}
