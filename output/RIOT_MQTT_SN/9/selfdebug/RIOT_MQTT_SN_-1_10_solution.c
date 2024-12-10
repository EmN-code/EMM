#include "net/emcute.h"
#include "net/gnrc.h"
#include "xtimer.h"
#include "msg.h"

#define EMCUTE_ID "esp32"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define PUB_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char *topic_name = "state";

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_sub_t sub;

    sub.topic.name = topic_name;   
    sub.cb = NULL; 
    
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to subscribe");
        return NULL;
    }
    
    while (1) {
        xtimer_sleep(5);
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish");
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    memset(&topic, 0, sizeof(topic));
    topic.name = topic_name;

    emcute_sub_t sub;
    sub.topic.name = topic_name;
    sub.cb = NULL;

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    thread_yield_higher(thread_pid);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;  
    }
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    return 0;
}