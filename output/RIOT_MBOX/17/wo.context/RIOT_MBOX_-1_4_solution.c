#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <ztimer.h>
#include <periph/xtimer.h>

#define INTERVAL  (1U * US_PER_SEC) // Interval for timer event
#define MSG_TYPE  (0x3456)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];
static kernel_pid_t main_pid;
static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE;
    msg.content.value = ztimer_now(ZTIMER_MSEC);
    printf("Sending message with timestamp %ld\n", msg.content.value);
    msg_send(&msg, main_pid);
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    main_pid = thread_getpid();

    // Initialize and set the timer
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, main_pid, MSG_TYPE);
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        msg_t response;
        msg_receive(&response);

        if (response.type == MSG_TYPE) {
            uint32_t timestamp = response.content.value;
            printf("Received message with timestamp %ld\n", timestamp);
        }
    }
    return 0;
}