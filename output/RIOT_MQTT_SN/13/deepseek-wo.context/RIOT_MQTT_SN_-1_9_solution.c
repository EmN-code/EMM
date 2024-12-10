
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG_SIZE 15
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_buf[MSG_SIZE];
static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void) {
    static int state = 0;
    const char *states[] = { "work", "not work" };

    snprintf(msg_buf, sizeof(msg_buf), "%s", states[state]);
    state = !state;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the broker

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&topic, msg_buf, strlen(msg_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg_buf);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Convert the broker IP address from string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }

    puts("Connected to the broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
