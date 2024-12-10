#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <msg.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <stdlib.h>
#include <time_units.h>

#define MESSAGE_INTERVAL   1000000 // 1 second
#define THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE         (KERNEL_CONF_STACKSIZE_PRINTF + THREAD_EXTRA_STACKSIZE_PRINTF)

static char thread_stack[STACK_SIZE];

void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_receive(&msg, &msg, thread_getpid());
    printf("Message received: %lu\n", msg.content.value);
    return NULL;
}

int main(void) {
    // Initialize a message queue
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Create a thread for message processing
    kernel_pid_t msg_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY, 0, message_thread, NULL, "msg_thread");

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
        msg_t msg;
        msg.content.value = xtimer_usec_from_tick(xtimer_now()).tick;

        if (msg_try_send(&msg, msg_pid) != 1) {
            printf("Failed to send message\n");
        }
    }

    return 0;
}