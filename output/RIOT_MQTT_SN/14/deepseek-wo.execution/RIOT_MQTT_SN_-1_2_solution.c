
#include <net/ipv6/addr.h>
#include <thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <event/thread.h>

#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

static emcute_sub_t sub;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publish_state(void *arg) {
    emcute_topic_t topic;
    int state = 0; // 0 for "work", 1 for "not work"

    // Register the topic
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    while (1) {
        const char *msg = (state == 0) ? MSG_WORK : MSG_NOT_WORK;
        emcute_pub(&topic, msg, strlen(msg), 0);
        state = !state; // Toggle state
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publish_state");

    return 0;
}
