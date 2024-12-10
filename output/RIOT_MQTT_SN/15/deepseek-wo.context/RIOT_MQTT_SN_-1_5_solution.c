
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received message on topic '%s' [%i]: %.*s\n",
           topic->name, (int)topic->id, (int)len, (char *)data);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345; /* Choose a random local port */

    /* Initialize the MQTT-SN topic */
    topic.name = TOPIC_NAME;
    topic.id = 0; /* Will be set by the gateway */

    /* Initialize the UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Convert IP address from string to binary form */
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    /* Register the topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    printf("Registered topic: %s\n", topic.name);

    /* Subscribe to the topic */
    emcute_sub_t sub = { .topic = &topic, .cb = _on_pub };
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }
    puts("Subscribed to topic");

    /* Publish state every 5 seconds */
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
