#include <stdio.h>
#include <ztimer.h>
#include <msg.h>

#define TIMER_INTERVAL (1000000U)  // Timer interval in microseconds
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    puts("Starting message box example...");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Message received!");
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
        ztimer_set_cb(ZTIMER_USEC, &timer, timer_callback, NULL);
    }

    return 0;
}