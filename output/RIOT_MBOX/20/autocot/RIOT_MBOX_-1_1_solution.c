#include <ztimer.h>  // For ztimer types and functions
#include <msg.h>     // For IPC message handling
#include <stdio.h>   // For standard input/output functions

#define TIMER_INTERVAL (1000000U)  // Timer interval in microseconds
#define QUEUE_SIZE (8)  // Size of the message queue

static msg_t msg_queue[QUEUE_SIZE];  // Message queue buffer

// Callback function for event timer
void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void) {
    evtimer_t timer;
    ztimer_t ztimer;
    kernel_pid_t main_pid = thread_getpid();
    evtimer_t *etimer = &timer;

    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    ztimer_set_msg(&ztimer, ZTIMER_USEC, TIMER_INTERVAL, &msg_queue, main_pid);

    // Configure event timer callback
    evtimer_init(etimer, &ztimer, NULL);
    evtimer_set_callback(etimer, timer_callback, (void *)main_pid);

    // Main message handling loop
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return 0;
}
