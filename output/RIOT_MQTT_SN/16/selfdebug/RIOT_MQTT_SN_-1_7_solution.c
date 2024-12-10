#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <stdio.h>

// Define the topic and message states
static char *state_topic = "state";
static char *msg_work = "work";
static char *msg_not_work = "not_work";

// Structure for the topic ID
static emcute_topic_t state_topic_id;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Initialize the emcute library and connect to the gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Set up topic
    state_topic_id.name = state_topic;
    if (emcute_reg(&state_topic_id) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Main loop to publish state every 5 seconds
    while (1) {
        // Publish "work" state
        if (emcute_pub(&state_topic_id, msg_work, strlen(msg_work), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);

        // Publish "not work" state
        if (emcute_pub(&state_topic_id, msg_not_work, strlen(msg_not_work), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        // Wait for 5 more seconds
        xtimer_sleep(5);
    }
    
    return 0;
}
