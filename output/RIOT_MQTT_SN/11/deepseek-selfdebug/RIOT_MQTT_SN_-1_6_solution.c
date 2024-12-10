
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>  // Include this header for inet_pton
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_payload[] = "work";

static void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic '%s'\n", TOPIC);
        return;
    }

    res = emcute_pub(&topic, msg_payload, strlen(msg_payload), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message\n");
    } else {
        printf("Published: %s\n", msg_payload);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = BROKER_PORT;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error: invalid broker IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker\n");
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    // Run the emcute thread
    emcute_run(BROKER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
