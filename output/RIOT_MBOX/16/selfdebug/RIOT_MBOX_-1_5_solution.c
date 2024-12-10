#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define MSG_QUEUE_SIZE 8
#define INTERVAL (1U * ZTIMER_USEC_PER_SEC)

static kernel_pid_t sched_active_pid = KERNEL_PID_UNDEF;  // Initialize with undefined
static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;

static void event_handler(void *arg) {
    (void)arg;
    printf("Handling event\n");
    msg_t msg;
    msg_receive(&msg);
    // Process msg here
}

int main(void) {
    sched_active_pid = thread_getpid();  // Correct way to get current PID in RIOT
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    timer.callback = event_handler;
    timer.arg = NULL;
    ztimer_set_timeout(ZTIMER_USEC, &timer, INTERVAL);
    ztimer_start(ZTIMER_USEC, &timer);  // Schedule the timer

    while (1) {
        msg_t msg;
        // Prepare message if needed then send it
        msg_send(&msg, sched_active_pid);
        printf("Event scheduled\n");
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return 0;
}