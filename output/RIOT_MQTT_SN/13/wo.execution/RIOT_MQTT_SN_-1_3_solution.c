#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t state_topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, sizeof(queue)/sizeof(msg_t));
    static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    
    gw.netif = SOCK_ADDR_ANY_NETIF;
    sock_ipaddr_str2u32(&gw.addr.ipv4, "47.102.103.1");

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    while (1) {
        const char *state = "work";
        emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    return 0;
}