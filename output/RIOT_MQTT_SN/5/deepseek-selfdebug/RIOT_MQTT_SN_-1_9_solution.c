
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the broker

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;  // Toggle between "work" and "not work"

    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", SERVER_IP, SERVER_PORT);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker\n");

    emcute_run(SERVER_PORT, CLIENT_ID);

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
