#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define INTERVAL (1000U * US_PER_MS)  // 1-second interval
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static ztimer_t _ztimer;

// Callback function for the timer
static void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = ztimer_now(ZTIMER_MSEC);
    msg_try_send(&msg, (kernel_pid_t)(uintptr_t)arg);
}

int main(void) {
    // Initialize message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the timer
    ztimer_set_msg(ZTIMER_MSEC, &_ztimer, INTERVAL, timer_callback, (void *)(uintptr_t)thread_getpid());

    printf("Timer set for sending events every %u milliseconds.\n", INTERVAL / 1000);

    // Handler loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received event message with content value: %d\n", msg.content.value);
    }

    return 0;
}