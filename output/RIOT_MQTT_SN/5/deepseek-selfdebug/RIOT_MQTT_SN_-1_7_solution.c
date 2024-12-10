
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t broker = { .port = BROKER_PORT };

static void publish_state(void)
{
    static char buf[64];
    static emcute_topic_t topic;

    strcpy(buf, MSG);
    topic.name = TOPIC;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return;
    }

    if (emcute_pub(&topic, buf, strlen(buf), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    int res;

    /* Initialize the network interface */
    if (netdev_default == NULL) {
        puts("Error: no default network interface found");
        return 1;
    }

    /* Initialize EMCUTE */
    emcute_init();

    /* Convert IP address from string to binary */
    if (inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
