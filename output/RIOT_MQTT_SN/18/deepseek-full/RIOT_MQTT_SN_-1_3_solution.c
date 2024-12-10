
#include "event/thread.h"
#include "net/emcute.h"
#include "thread.h"
#include <netinet/in.h>
#include "net/ipv6/addr.h"

#define STACKSIZE (512)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define TOPIC "state"
#define STATE_MSG "work"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT (1883)
#define INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[STACKSIZE];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return NULL;
    }

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG) - 1, 0) != EMCUTE_OK) {
            continue;
        }
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    return 0;
}
