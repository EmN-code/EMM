
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define TOPIC "state"
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_buf[MSG_SIZE];
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    snprintf(msg_buf, sizeof(msg_buf), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_topic_t topic;
    topic.name = TOPIC;
    if (emcute_reg(&topic) == EMCUTE_OK) {
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
    } else {
        puts("Error: unable to register topic");
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Set the gateway IP address
    IP4_ADDR(&gw.addr.ipv4, 47, 102, 103, 1);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
