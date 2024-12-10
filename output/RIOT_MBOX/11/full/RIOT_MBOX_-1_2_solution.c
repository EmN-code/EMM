#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <evtimer_mbox.h>

#define INTERVAL (1U * US_PER_SEC)  // Interval of 1 second

static char stack[THREAD_STACKSIZE_MAIN];

void *sender_function(void *arg) {
    (void) arg;
    evtimer_msg_event_t event;
    event.period = INTERVAL;
    kernel_pid_t receiver_pid = thread_getpid();

    evtimer_msg_oneshot(&event, receiver_pid, 0);

    while (1) {
        if (event.timer == 0) {
            evtimer_msg_oneshot(&event, receiver_pid, 0);
        }
        msg_t msg;
        msg_receive(&msg);
        printf("Message received with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sender_function, NULL, "sender");

    while (1) {
        xtimer_sleep(5); // Sleep for 5 seconds, not using ztimer_sleep
        printf("Main thread running...\n");
    }

    return 0;
}