
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";

static void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0; // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic\n");
        return;
    }

    res = emcute_pub(&topic, state_msg, sizeof(state_msg) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message\n");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker\n");
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}