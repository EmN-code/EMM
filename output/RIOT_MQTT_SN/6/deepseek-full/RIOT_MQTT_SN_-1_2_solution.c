
#include "event/thread.h"
#include "net/emcute.h"
#include "thread.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[STACKSIZE];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void *emcute_thread(void *arg)
{
    (void)arg;

    // Set the gateway address
    ipv4_addr_t gw_addr = { .u32 = 0x6767662f }; // 47.102.103.1 in hex
    gw.addr.ipv4 = gw_addr;

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN gateway");
        return NULL;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return NULL;
    }

    // Publish state every 5 seconds
    char state[] = "work";
    while (1) {
        res = emcute_pub(&topic, state, sizeof(state) - 1, 0);
        if (res != EMCUTE_OK) {
            puts("Error: Could not publish state");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Create the EMCUTE thread
    thread_create(stack, sizeof(stack), PRIORITY, 0, emcute_thread, NULL, "emcute");

    return 0;
}
