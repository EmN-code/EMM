
#include "evtimer_msg.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "timex.h"
#include "stdatomic.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "event/thread.h"
#include "stdbool.h"
#include "MQTTClient.h"
#include "irq.h"
#include "msg.h"
#include "thread.h"
#include "mbox.h"
#include "stddef.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static volatile atomic_int isr_flag = 0;

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_t reply;
    reply.content.value = 0;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        switch (msg.content.value) {
            case 1:
                printf("Interrupt handled successfully\n");
                break;
            case 2:
                printf("Interrupt handling failed\n");
                break;
            default:
                printf("Unknown message received\n");
                break;
        }
        msg_reply(&msg, &reply);
    }

    return NULL;
}

static void isr_handler(void)
{
    atomic_store(&isr_flag, 1);
}

int main(void)
{
    thread_create(
        _main_msg_queue,
        sizeof(_main_msg_queue),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    unsigned state = irq_disable();
    isr_handler();
    irq_restore(state);

    if (atomic_load(&isr_flag) == 1) {
        msg_t msg;
        msg.content.value = 1;
        msg_send(&msg, thread_getpid());
    } else {
        msg_t msg;
        msg.content.value = 2;
        msg_send(&msg, thread_getpid());
    }

    return 0;
}
