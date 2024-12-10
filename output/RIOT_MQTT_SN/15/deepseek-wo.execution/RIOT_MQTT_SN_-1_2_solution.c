
#include <net/ipv6/addr.h>
#include <thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <event/thread.h>

#define TOPIC_NAME "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MSG_SIZE 128
#define STACK_SIZE 1024
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_pub(&topic, msg, strlen(msg), 0);
}

void *emcute_thread(void *arg) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), PRIORITY, 0, emcute_thread, NULL, "emcute");
    return 0;
}
