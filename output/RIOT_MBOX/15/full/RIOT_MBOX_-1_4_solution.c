#include <xtimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <thread.h>
#include <timex.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    mbox_t my_mbox;
    mbox_init(&my_mbox, queue, 8);

    while (1) {
        xtimer_sleep(1); // Pause for a second
        msg.content.value = 42; // Example message
        if(mbox_send(&my_mbox, &msg) == 1) {
            printf("Message sent successfully\n");
        } else {
            printf("Message sending failed\n");
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                                               thread_handler, NULL, "receiver");
    (void)receiver_pid;  // Suppress unused variable warning
    printf("Thread created, running typing test\n");
    return 0;
}