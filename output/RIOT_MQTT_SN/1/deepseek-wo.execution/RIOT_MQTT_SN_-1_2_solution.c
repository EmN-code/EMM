
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define MQTT_SN_BROKER_ADDR "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV6_EP_ANY;

static void _publish_state(void)
{
    static bool state = false;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void)
{
    /* Initialize the MQTT-SN client */
    emcute_init();

    /* Set up the gateway endpoint */
    gw_ep.family = AF_INET;
    gw_ep.port = MQTT_SN_BROKER_PORT;
    inet_pton(AF_INET, MQTT_SN_BROKER_ADDR, &gw_ep.addr.ipv4);

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    /* Register the topic */
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
