
#include <evtimer_msg.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <irq.h>
#include <msg.h>
#include <net/emcute.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 512
#define PRIORITY 5

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);
static msg_t msg_queue[8];
static mbox_t mbox;

void worker_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        if (msg.content.value == 1) {
            atomic_store(&interrupt_flag, 0);
        }
    }
}

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
    msg_t msg;
    msg.content.value = 1;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), PRIORITY, 0, worker_thread, NULL, "worker");

    while (1) {
        if (atomic_load(&interrupt_flag) == 1) {
            interrupt_handler();
        }
        xtimer_sleep(1);
    }

    return 0;
}
