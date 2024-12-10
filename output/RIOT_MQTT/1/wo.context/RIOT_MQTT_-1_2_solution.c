#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/af.h>
#include <periph/gpio.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U)

static emcute_sock_t sock;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published message successfully!\n");
}

int main(void) {
    char *message = "work or not work";
    emcute_topic_t topic;
    emcute_sub_t subscription;

    /* convert broker address */
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    /* initialize emcute socket */
    memset(&sock, 0, sizeof(sock));
    emcute_connect(&sock, &gw, false, on_pub, NULL);

    /* configure topic */
    topic.name = "state";
    emcute_reg(&topic);
    topic.id = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);

    while (1) {
        xtimer_sleep(PUB_INTERVAL);
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        printf("Published state message\n");
    }

    return 0;
}