
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void publish_state(void)
{
    static int state = 0;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    // Initialize the EMCUTE stack
    emcute_run(1883, "riot_client");

    // Set the gateway address
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
