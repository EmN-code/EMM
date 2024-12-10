
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[20];
static emcute_topic_t topic;

void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Prepare the message
    snprintf(state_msg, sizeof(state_msg), "%s", states[state]);
    state = !state; // Toggle state

    // Publish the message
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_t server_addr;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }
    gw.addr.ipv4 = server_addr;

    // Initialize the MQTT-SN client
    emcute_run(SERVER_PORT, CLIENT_ID);

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
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
