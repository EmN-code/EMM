#include <timers/ztimer.h>   // for ztimer
#include <xtimer.h>   // for xtimer
#include <stdio.h>   // for printf
#include <msg.h>   // for msg_t, msg_receive, etc.

#define MSG_TYPE_EVENT (0x01)
#define INTERVAL_USEC (1000000) // 1-second interval
#define MAIN_QUEUE_SIZE (8)

static msg_t main_queue[MAIN_QUEUE_SIZE];

void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_t timer;
    msg.type = MSG_TYPE_EVENT;

    ztimer_init(ZTIMER_MSEC);
    msg.content.value = 0;

    ztimer_ticks32_t interval = ztimer_now(ZTIMER_MSEC) + INTERVAL_USEC / 1000;
    ztimer_set(ZTIMER_MSEC, &timer, interval);

    while (1) {
        if (ztimer_elapsed(interval)) {
            msg.content.value++;
            printf("Sending event number: %d\n", msg.content.value);
            msg_send(&msg, thread_getpid());
            // Reset the timer
            interval = ztimer_now(ZTIMER_MSEC) + INTERVAL_USEC / 1000;
        }
        ztimer_sleep(ZTIMER_MSEC, 100);
    }
    return NULL;
}

int main(void) {
    puts("RIOT ESP32 Message Box Example");

    msg_init_queue(main_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t event_thr = thread_create(NULL, 512, ZTIMER_MSEC, 0, event_thread, NULL, "event_thread");
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_EVENT) {
            printf("Received event number: %d\n", msg.content.value);
        }
    }

    return 0;
}