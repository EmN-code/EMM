
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 512
#define PRIORITY 5

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);
static msg_t msg_queue[8];
static mbox_t mbox;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
    msg_t msg;
    msg.content.value = 1;
    msg_send(&msg, thread_getpid());
}

void worker_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.content.value == 1) {
            printf("Interrupt received and handled.\n");
            atomic_store(&interrupt_flag, 0);
        }
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), PRIORITY, 0, worker_thread, NULL, "worker");

    xtimer_t timer;
    timer.callback = interrupt_handler;
    xtimer_set(&timer, 1000000); // 1 second

    while (1) {
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt flag is set.\n");
        }
        xtimer_sleep(1);
    }

    return 0;
}
