#include <xtimer.h>
#include <stdio.h>
#include <kernel.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_entry(void *arg) {
    (void)arg;
    msg_t msg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while(1) {
        msg_send(&msg, thread_getpid());
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
        printf("Event sent!\n");
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    printf("Message box example\n");

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_entry, NULL, "thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Event received!\n");
    }
    return 0;
}