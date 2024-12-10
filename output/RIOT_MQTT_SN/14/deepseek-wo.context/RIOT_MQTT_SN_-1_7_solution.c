
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t t;

void publish_state(void) {
    char buf[64];
    sprintf(buf, "%s", MSG);
    emcute_pub(&t, buf, strlen(buf), EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: unable to convert IP address");
        return 1;
    }

    if (sock_udp_create(&local, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
