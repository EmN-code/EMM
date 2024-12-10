#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <string.h>

#define SERVER_PORT     1883
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"

static emcute_topic_t topic;

void publish_state(const char *state) {
    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("[Error] publishing failed with code %d\n", res);
    }
}

int main(void) {
    struct sockaddr_in sa;
    emcute_con((sock_udp_ep_t *)&sa, true, NULL, NULL, 0, 0);

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state("work");
        xtimer_sleep(5);
    }
    return 0;
}