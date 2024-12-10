
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t t;

void publish_state(void)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%s", MSG);
    emcute_pub(&t, buf, strlen(buf), 0);
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    /* Initialize the network stack */
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Convert IP address string to byte array */
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    /* Register topic */
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    puts("Topic registered");

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
