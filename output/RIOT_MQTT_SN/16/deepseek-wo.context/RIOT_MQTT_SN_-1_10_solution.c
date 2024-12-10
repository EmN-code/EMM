
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

static void _publish_state(void)
{
    int res = emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", MESSAGE);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    puts("Topic registered");

    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
