
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    char payload[16];

    // Register the topic
    if (emcute_reg(TOPIC_NAME, &topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Prepare the payload
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    // Publish the state
    if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Run the EMCUTE thread
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
