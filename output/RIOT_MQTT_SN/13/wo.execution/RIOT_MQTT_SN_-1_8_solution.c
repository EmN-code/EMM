#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "string.h"
#include "msg.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "net/emcute.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    /* not used in this example */
}

int main(void)
{
    msg_init_queue(queue, 8);

    puts("RIOT MQTT-SN Example
");

    kernel_pid_t emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                                             emcute_run, NULL, "emcute");
    if (emcute_pid <= KERNEL_PID_UNDEF) {
        puts("error: unable to start emcute thread");
        return 1;
    }

    sock_udp_ep_t gw;
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = 1883;

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    emcute_topic_t t;
    strcpy(t.name, "state");

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    while (1) {
        const char *state = "work or not work";
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        xtimer_sleep(5);
    }

    return 0;
}