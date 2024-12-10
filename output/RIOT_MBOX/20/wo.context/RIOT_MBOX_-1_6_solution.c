#include "msg.h"
#include "kernel.h"
#include "stdio.h"
#include "ztimer.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;
static void timer_callback(void *arg) {
    puts("Timer callback triggered, sending message...");

    msg_t msg;
    msg.content.value = 42;
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void) {
    printf("RIOT message box example\n");

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, &msg_queue[0], thread_getpid());
    timer.callback = timer_callback;
    timer.arg = (void *)thread_getpid();
    ztimer_start(&timer);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}