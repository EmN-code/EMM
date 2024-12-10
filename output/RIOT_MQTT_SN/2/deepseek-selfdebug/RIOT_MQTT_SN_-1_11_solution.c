
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res == EMCUTE_OK) {
        printf("Published state: %s\n", state);
    } else {
        printf("Failed to publish state: %s (error code: %d)\n", state, res);
    }
}

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = SERVER_PORT;

    int res = inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker (error code: %d)\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (error code: %d)\n", res);
        emcute_discon();
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}
