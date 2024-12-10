
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

#define QUEUE_SIZE 8
#define PRIORITY   THREAD_PRIORITY_MAIN - 1
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
        msg.content.value++;
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
