#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL_SEC  (2)

static msg_t _msg_queue[MAIN_QUEUE_SIZE];
static kernel_pid_t example_thread_pid;

void *example_thread(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u and value: %lu\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    ztimer_t timer;
    msg_t msg;
    msg.content.value = 42; // Example message content

    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);

    example_thread_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, example_thread, NULL, "example_thread");

    ztimer_set(ZTIMER_SECOND, &timer, TIMER_INTERVAL_SEC * ZTIMER_MSEC_PER_SEC);

    while (1) {
        ztimer_sleep(ZTIMER_SECOND, TIMER_INTERVAL_SEC * ZTIMER_MSEC_PER_SEC);
        msg.type = 0x100; // Example message type
        msg_try_send(&msg, example_thread_pid);
    }

    return 0;
}