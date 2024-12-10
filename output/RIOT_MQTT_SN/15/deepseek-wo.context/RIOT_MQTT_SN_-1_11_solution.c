
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work or not work";
static emcute_topic_t topic;

static void publish_state(void)
{
    int res = emcute_pub(&topic, state_msg, strlen(state_msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    int res = inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);
    if (res != 1) {
        puts("Error parsing broker IP address");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
