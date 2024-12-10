#include <ztimer.h>
#include <msg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE (8)
#define TIMER_INTERVAL (1000000U) // Interval in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t msg_thread_pid;
static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1; // Example message type
    msg.content.ptr = "Event occurred";
    printf("Sending message: %s\n", (char *)msg.content.ptr);
    msg_send(&msg, msg_thread_pid);
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL / 1000);
}

static void *msg_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t m;
    while (1) {
        msg_receive(&m);
        printf("Received message: %s\n", (char *)m.content.ptr);
    }
    return NULL;
}

int main(void) {
    printf("Message box demonstration\n");
    msg_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST, msg_thread, NULL, "msg_thread");
    ztimer_set_callback(ZTIMER_MSEC, &timer, timer_callback, NULL);
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL / 1000);
    return 0;
}