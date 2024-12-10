
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[20];

static void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    sprintf(state_msg, "%s", states[state]);
    state = !state;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    if (emcute_reg(&topic) == EMCUTE_OK) {
        emcute_pub(&topic, state_msg, strlen(state_msg), 0);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    emcute_discon();
    return 0;
}
