#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"

#define _IPV4         "47.102.103.1"
#define PORT         (1883U)
#define STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define MSG_TYPE_WORK    "work"
#define MSG_TYPE_NOT_WORK "not work"

static char stack[STACK_SIZE];
static emcute_sub_t subscriptions[1];
static char *topics[] = { "status" };

static void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = PORT};
    gw.addr.ipv4.u32 = inet_addr(_IPV4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d
", _IPV4, (int)PORT);
        return NULL;
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        const char *state = MSG_TYPE_WORK;
        t.name = topics[0];
        res = emcute_reg(&t);
        if (res != EMCUTE_OK) {
            printf("Error: unable to obtain topic ID
");
            break;
        }

        res = emcute_pub(&t, state, strlen(state), flags);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data
");
            break;
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN client application");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute_thread");
    return 0;
}