
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <timex.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 2048
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

volatile bool isr_flag = false;

void isr_handler(void)
{
    isr_flag = true;
}

void event_loop(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (isr_flag) {
            printf("Interrupt handled successfully.\n");
            isr_flag = false;
        } else {
            printf("Interrupt handling failed.\n");
        }
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, (thread_task_func_t)event_loop, NULL, "event_loop");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread.\n");
        return 1;
    }

    // Simulate an interrupt
    isr_handler();

    // Send a message to the event loop thread to trigger processing
    msg_t msg;
    msg_send(&msg, pid);

    return 0;
}
