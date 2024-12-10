
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[20];

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    sprintf(state_msg, "%s", states[state]);
    state = 1 - state;  // Toggle state

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the broker

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv4_addr_t broker_addr;

    if (inet_pton(AF_INET, BROKER_ADDR, &broker_addr) <= 0) {
        puts("Error: invalid broker address");
        return 1;
    }

    gw.addr.ipv4 = broker_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
