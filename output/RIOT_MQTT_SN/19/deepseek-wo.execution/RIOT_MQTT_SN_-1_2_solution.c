
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/af.h>
#include <event/thread.h>
#include <thread.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static char stack[STACKSIZE];
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    }
}

void *emcute_thread(void *arg) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv4_addr_t broker_addr;
    ipv4_addr_from_str(&broker_addr, BROKER_ADDR);
    gw.addr.ipv4 = broker_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_usleep(5 * US_PER_SEC); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), PRIORITY, 0, emcute_thread, NULL, "emcute");
    return 0;
}
