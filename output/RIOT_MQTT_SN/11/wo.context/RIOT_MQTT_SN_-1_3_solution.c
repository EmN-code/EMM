#include <xtimer.h>
#include <string.h>
#include <kernel_defines.h>
#include <emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         1883
#define PUB_TOPIC           "device/state"
#define PUB_INTERVAL        5000000  // 5 seconds

static emcute_topic_t my_topic;

int main(void) {
    // Setup the broker address
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: Unable to connect to the broker.");
        return 1;
    }

    // Prepare the topic structure
    my_topic.name = PUB_TOPIC;
    my_topic.id = 0;
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        puts("ERROR: Unable to register topic.");
        return 1;
    }

    // Message loop
    while (1) {
        const char *state_message = "work"; // default message, update as needed
        if (emcute_pub(&my_topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: Unable to publish message.");
        } else {
            puts("Message successfully published.");
        }
        xtimer_usleep(PUB_INTERVAL);
    }
    return 0;
}