
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_topic_t topic;

static void _publish_state(void)
{
    const char *msg = MESSAGE;
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_pub(&topic, msg, strlen(msg), flags) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

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
    puts("Topic registered successfully");

    while (1) {
        _publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
