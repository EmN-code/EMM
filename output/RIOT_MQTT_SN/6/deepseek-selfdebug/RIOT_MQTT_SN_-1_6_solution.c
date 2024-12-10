
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_CLIENT_ID "esp32_client"
#define MQTT_SN_TOPIC "state"
#define MQTT_SN_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void _publish_state(void)
{
    char msg[] = MQTT_SN_MESSAGE;
    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
}

int main(void)
{
    // Initialize the UDP socket endpoint for the gateway
    gw.family = AF_INET;
    gw.port = MQTT_SN_BROKER_PORT;
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Register the topic
    topic.name = MQTT_SN_TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
