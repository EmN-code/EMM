
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

static char stack[STACK_SIZE];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

void *publish_thread(void *arg) {
    (void)arg;
    const char *state_msg = "work";
    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        int res = emcute_pub(&topic, state_msg, strlen(state_msg), flags);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        }
        thread_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Set the gateway address
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Create and start the publish thread
    thread_create(stack, sizeof(stack), PRIORITY, 0, publish_thread, NULL, "publish_thread");

    return 0;
}
