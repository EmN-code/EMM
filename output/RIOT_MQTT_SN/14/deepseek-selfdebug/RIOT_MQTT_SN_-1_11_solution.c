
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .port = EMCUTE_PORT };
static emcute_topic_t t;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    // This function is defined but not used, so we can leave it empty.
}

int main(void)
{
    sock_udp_ep_t remote = { .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_ADDR, &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    char msg[20];
    while (1) {
        strcpy(msg, "work");
        if (emcute_pub(&t, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_usleep(INTERVAL);

        strcpy(msg, "not work");
        if (emcute_pub(&t, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
