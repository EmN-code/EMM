
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic; (void)data; (void)len;
    printf("Message published\n");
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;
    emcute_sub_t sub;
    emcute_topic_t t;

    /* Initialize UDP socket */
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Set broker address */
    if (inet_pton(AF_INET, BROKER_ADDR, &broker.addr.ipv4) <= 0) {
        puts("Error converting address");
        return 1;
    }

    /* Connect to MQTT-SN broker */
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    /* Register topic */
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Subscribe to topic */
    sub.topic = t;
    sub.cb = _on_pub;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&t, MSG, strlen(MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        xtimer_sleep(5);
    }

    /* Disconnect from broker */
    emcute_discon();

    return 0;
}
