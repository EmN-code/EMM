
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <ztimer.h>

#define STACK_SIZE 256
#define PRIORITY 5

static volatile bool interrupt_handled = false;
static volatile int atomic_var = 0;

static void *interrupt_handler_thread(void *arg)
{
    (void)arg;

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == 1) {
            atomic_var = 1;
            interrupt_handled = true;
        }
    }

    return NULL;
}

int main(void)
{
    msg_t msg_queue[8];
    mbox_t mbox = MBOX_INIT(msg_queue, 8);

    char stack[STACK_SIZE];
    thread_t *handler_thread = thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, interrupt_handler_thread, NULL, "interrupt_handler");

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000);

        if (!interrupt_handled) {
            msg_t msg;
            msg.type = 1;
            msg_send(&msg, handler_thread);
        } else {
            printf("Atomic variable modified correctly: %d\n", atomic_var);
            interrupt_handled = false;
        }
    }

    return 0;
}
