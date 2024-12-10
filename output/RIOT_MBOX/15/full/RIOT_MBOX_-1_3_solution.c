#include <xtimer.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <thread.h>
#include <timex.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static mbox_t my_mbox;
static msg_t my_msg_queue[8];  // Must be a power of two

void *receiver_loop(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received msg type: %d, content: %d\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, my_msg_queue, 8);
    kernel_pid_t receiver_pid = thread_create(
        (char *)calloc(1, THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, 0, receiver_loop, NULL, "receiver");

    msg_t msg;
    for (;;) {
        msg.type = 0x1234;
        msg.content.value = 5678;
        mbox_send(&my_mbox, &msg);
        xtimer_sleep(1);
    }

    return 0;
}