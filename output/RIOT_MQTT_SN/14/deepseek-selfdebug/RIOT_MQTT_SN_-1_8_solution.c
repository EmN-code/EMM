
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received message on topic %s: %.*s\n", topic->name, (int)len, (char *)data);
}

static int _publish_state(void)
{
    return emcute_pub(&topic, state_msg, strlen(state_msg), 0);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv4_addr_t broker_addr;

    if (ipv4_addr_from_str(&broker_addr, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker IP address");
        return 1;
    }

    gw.addr.ipv4 = broker_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    puts("Topic registered");

    while (1) {
        if (_publish_state() != EMCUTE_OK) {
            puts("Error: unable to publish state");
        } else {
            puts("State published");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
