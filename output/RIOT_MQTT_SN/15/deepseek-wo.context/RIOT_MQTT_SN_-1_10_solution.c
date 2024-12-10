
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define TOPIC_NAME "state"
#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[20];
static emcute_topic_t topic;

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Toggle state
    state = !state;
    strcpy(state_msg, states[state]);

    // Publish the state
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published state: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = MQTT_SERVER_PORT };
    emcute_sub_t sub;

    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    // Convert IP address from string to binary
    if (inet_pton(AF_INET, MQTT_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
