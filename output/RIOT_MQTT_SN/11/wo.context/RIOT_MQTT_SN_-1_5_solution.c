#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "msg.h"
#include "xtimer.h"

#define EMCUTE_ID      ("gerald")
#define EMCUTE_PORT    (1883U)
#define EMCUTE_PRIO    (THREAD_PRIORITY_MAIN - 1)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static char buf[64];

static int publish(const char *topic_name, const char *data)
{
    unsigned flags = EMCUTE_QOS_0;  // use QoS 0
    topic.name = topic_name;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        return res;
    }

    res = emcute_pub(&topic, data, strlen(data), flags);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data to topic.\n");
        return res;
    }

    return EMCUTE_OK;
}

void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);  // convert IP address

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker.\n");
        return NULL;
    }

    while (1) {
        publish("device/state", "work");
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    /* start emcute in its own thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}