#include "string.h"
#include "stdio.h"
#include "mma8x5x_regs.h"
#include "stdlib.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define NUMOFSUBS 16
#define TOPIC_MAXLEN 64

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char* state_topic = "device/state";

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        char *work_state = "work";
        emcute_topic_t t;
        unsigned flags = EMCUTE_QOS_0;

        t.name = state_topic;
        if (emcute_reg(&t) != EMCUTE_OK) {
            printf("Error: unable to register topic\n");
            continue;
        }

        if (emcute_pub(&t, work_state, strlen(work_state), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published '%s' on topic '%s'\n", work_state, t.name);
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET6, /* assuming IPv6 */
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    emcute_run(BROKER_PORT, "riot_client");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return 1;
    }

    printf("Connected to [%s]:%d\n", BROKER_IP, BROKER_PORT);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher_thread");

    return 0;
}