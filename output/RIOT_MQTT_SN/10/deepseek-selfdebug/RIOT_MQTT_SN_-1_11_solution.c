
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[20];
static emcute_topic_t topic;

static void _publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Toggle state
    state = 1 - state;

    // Prepare the message
    strncpy(state_msg, states[state], sizeof(state_msg) - 1);
    state_msg[sizeof(state_msg) - 1] = '\0';

    // Publish the message
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Initialize the EMCUTE stack
    emcute_run(MQTT_BROKER_PORT, MQTT_CLIENT_ID);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
