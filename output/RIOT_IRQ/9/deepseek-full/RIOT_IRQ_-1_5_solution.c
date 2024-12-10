
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <irq.h>
#include <thread.h>
#include <evtimer_mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 512
#define PRIORITY 5

static atomic_int interrupt_handled = ATOMIC_VAR_INIT(0);
static msg_t msg_queue[8];
static mbox_t mbox;

void interrupt_handler(void *arg) {
    (void)arg;
    atomic_store(&interrupt_handled, 1);
    printf("Interrupt handled!\n");
}

void thread_function(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.content.value == 1) {
            interrupt_handler(NULL);
        }
    }
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    char stack[STACKSIZE];
    thread_create(stack, STACKSIZE, PRIORITY, 0, thread_function, NULL, "thread");

    while (1) {
        if (atomic_load(&interrupt_handled) == 0) {
            msg_t msg;
            msg.content.value = 1;
            msg_send(&msg, thread_getpid());
        } else {
            atomic_store(&interrupt_handled, 0);
            printf("Interrupt reset!\n");
        }
        xtimer_sleep(1);
    }

    return 0;
}
