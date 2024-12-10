
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw;

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
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    /* Register the topic */
    res = emcute_reg(&topic, TOPIC_NAME);
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
