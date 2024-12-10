#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUB_INTERVAL (5U * 1000000U) // 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

void publish_state(void) {
    const char* state_msg = "work";
    int res = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the broker\n");
        return 1;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}