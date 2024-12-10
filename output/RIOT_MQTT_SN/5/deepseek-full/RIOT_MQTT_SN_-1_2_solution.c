
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void _publish_state(void)
{
    static bool state = false;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: Failed to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    /* Initialize the UDP endpoint for the gateway */
    if (sock_udp_str2ep(&gw, SERVER_ADDR) < 0) {
        puts("Error: Invalid gateway address");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Failed to connect to MQTT-SN gateway");
        return 1;
    }

    /* Register the topic */
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Failed to register topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
