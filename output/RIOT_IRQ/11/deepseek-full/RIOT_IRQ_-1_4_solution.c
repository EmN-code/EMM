
#include <evtimer_msg.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 512

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_t msg_queue[8];
    mbox_t mbox;
    evtimer_t evtimer;
    evtimer_msg_event_t ev;

    mbox_init(&mbox, msg_queue, 8);
    evtimer_init_mbox(&evtimer, &mbox);

    evtimer_add_msg(&evtimer, &ev, 0, (void *)interrupt_handler, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);

        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt handled successfully!\n");
            atomic_store(&interrupt_flag, 0);
        }
    }

    return 0;
}
