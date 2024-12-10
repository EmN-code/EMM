
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define STATE_MESSAGE "work"

static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    const char *message = STATE_MESSAGE;
    int res = emcute_pub(&topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    // Initialize the UDP socket for communication with the broker
    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_create(&sock, &local, NULL, 0);

    // Convert the broker IP address from string to binary form
    inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
