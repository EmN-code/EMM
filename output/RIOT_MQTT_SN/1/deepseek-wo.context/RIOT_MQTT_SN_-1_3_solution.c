
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work or not work";
static emcute_topic_t topic;

static void _register_topic(void) {
    topic.name = TOPIC_NAME;
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: unable to register topic");
    } else {
        printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);
    }
}

static void _publish_state(void) {
    int res = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Published state message");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    _register_topic();

    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
