
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID          "riot_esp32"
#define EMCUTE_TOPIC       "state"
#define EMCUTE_MSG         "work or not work"
#define EMCUTE_INTERVAL    (5U)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void publish_state(void)
{
    int res;
    res = emcute_pub(&topic, EMCUTE_MSG, strlen(EMCUTE_MSG), 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish message (%d)\n", res);
    }
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_t sock;
    int res;

    /* Initialize the socket */
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("error initializing socket");
        return 1;
    }

    /* Set the gateway address */
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Connect to the MQTT-SN broker */
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect to (%s)\n", gw.addr.ipv4);
        return 1;
    }

    /* Register the topic */
    topic.name = EMCUTE_TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic (%d)\n", res);
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(EMCUTE_INTERVAL);
    }

    /* Close the socket */
    sock_udp_close(&sock);

    return 0;
}
