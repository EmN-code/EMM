
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the broker

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: Failed to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv4_addr_t broker_addr;

    if (inet_pton(AF_INET, BROKER_IP, &broker_addr) <= 0) {
        puts("Error: Invalid broker IP address");
        return 1;
    }
    gw.addr.ipv4 = broker_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
