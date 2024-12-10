
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC "state"
#define MQTT_SN_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char state_msg[] = "work";

static void _publish_state(void)
{
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = MQTT_SN_TOPIC;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic '%s' [%d]\n", topic.name, res);
        return;
    }

    res = emcute_pub(&topic, state_msg, strlen(state_msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message [%d]\n", res);
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    int res = inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);
    if (res <= 0) {
        printf("Error: invalid broker IP address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker [%d]\n", res);
        return 1;
    }

    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
