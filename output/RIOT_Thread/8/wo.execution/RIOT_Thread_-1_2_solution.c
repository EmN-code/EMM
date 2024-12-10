#include <thread.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>

#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with type: %u and value: %d\n", msg.type, msg.content.value);
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    kernel_pid_t thread_pid = thread_create(
        thread_stack,
        sizeof(thread_stack),
        THREAD_PRIORITY_MAIN,
        CREATE_STACKTEST,
        thread_function,
        NULL,
        "my_thread"
    );

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    msg_send(&msg, thread_pid);

    return 0;
}