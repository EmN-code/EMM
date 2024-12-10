
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work or not work"

static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t t;

static void on_pub(void *arg, emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic '%s' [%i]\n", topic->name, (int)topic->id);
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error parsing IPv4 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&t, MSG, strlen(MSG), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    return 0;
}
