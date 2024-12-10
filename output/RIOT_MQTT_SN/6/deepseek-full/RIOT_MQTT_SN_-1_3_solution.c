
#include "event/thread.h"
#include "net/emcute.h"
#include "thread.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define STACK_SIZE          (1024)
static char _emcute_stack[STACK_SIZE];

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define TOPIC_NAME          "device/state"
#define STATE_MSG           "work or not work"

static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(0, "emcute");
    return NULL;
}

static int connect_to_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    ipv4_addr_t gw_addr = { .u32 = IPv4_ADDR(47, 102, 103, 1) };
    gw.addr.ipv4 = gw_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return -1;
    }
    return 0;
}

static int register_topic(void)
{
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        return -1;
    }
    return 0;
}

static int publish_state(void)
{
    if (emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG) - 1, 0) != EMCUTE_OK) {
        return -1;
    }
    return 0;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(_emcute_stack, sizeof(_emcute_stack), EMCUTE_PRIO,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    if (connect_to_broker() != 0) {
        puts("Failed to connect to MQTT-SN broker");
        return 1;
    }

    if (register_topic() != 0) {
        puts("Failed to register topic");
        return 1;
    }

    while (1) {
        if (publish_state() != 0) {
            puts("Failed to publish state");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
