
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[20];
static emcute_topic_t topic;

static void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    sprintf(state_msg, "%s", states[state]);
    state = !state;  // Toggle state

    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    uint8_t addr[sizeof(struct in_addr)];

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, addr) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }
    gw.addr.ipv4.u8 = addr;
    gw.family = AF_INET;

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
    printf("Registered topic: %s\n", topic.name);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
