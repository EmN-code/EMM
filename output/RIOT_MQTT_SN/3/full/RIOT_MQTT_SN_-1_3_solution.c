#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define EMCUTE_ID          "gertrud"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static const sock_udp_ep_t server = {
    .family = AF_INET6,
    .port = EMCUTE_PORT
};

static void *emcute_thread(void *arg)
{
    (void)arg;
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    emcute_con(&server, true, NULL, NULL, 0, 0);

    emcute_topic_t topic;
    topic.name = "device/state";
    emcute_reg(&topic);

    while (1) {
        const char *state = "work";
        emcute_pub(&topic, state, strlen(state), 0);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    return 0;
}