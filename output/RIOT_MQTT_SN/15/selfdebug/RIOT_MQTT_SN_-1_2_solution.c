#include "xtimer.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_INTERVAL    (5U * US_PER_SEC) // Publish every 5 seconds

static sock_udp_ep_t broker_endpoint;
static emcute_topic_t topic;
static char topic_name[] = "device/state";

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);
    
    broker_endpoint.family = AF_INET;
    inet_pton(AF_INET, BROKER_ADDRESS, &broker_endpoint.addr.ipv4);
    broker_endpoint.port = BROKER_PORT;

    if (emcute_con(&broker_endpoint, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", message);
        }

        xtimer_sleep(5);
    }

    return 0;
}