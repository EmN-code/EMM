#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define MESSAGE_QUEUE_SIZE 8
#define TIMER_INTERVAL_MS 1000

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static kernel_pid_t main_thread_pid;

static void _timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234; // Use a specific message type
    printf("Timer callback: Sending message with type: 0x%04x\n", msg.type);
    msg_send(&msg, main_thread_pid);
}

int main(void) {
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);
    main_thread_pid = thread_getpid();

    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL_MS, _timer_callback, NULL);
    printf("Timer set to trigger every %d ms\n", TIMER_INTERVAL_MS);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread: Received message with type: 0x%04x\n", msg.type);
    }

    return 0;
}