
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static emcute_topic_t topic;

static void publish_state(void)
{
    const char *message = STATE_MESSAGE;
    emcute_pub(&topic, message, strlen(message), 0);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
