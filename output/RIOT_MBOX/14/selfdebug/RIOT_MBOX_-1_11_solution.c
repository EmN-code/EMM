#include <thread.h>
#include <msg.h>
#include <event.h>

#define US_PER_MS 1000

void event_handler(void *arg) {
    (void) arg;
    msg_t msg;
    msg_receive(&msg);
    // Handle the received message (event)
}

int main(void) {
    static msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, event_handler, NULL, "event_thread"
    );

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error creating the event_handler thread\n");
        return 1;
    }

    evtimer_t evtimer;
    evtimer_msg_event_t event;
    event.msg.type = 0;  // Define an appropriate message type

    evtimer_init_msg(&evtimer);

    int interval = US_PER_MS * 500;  // 500 ms
    while (1) {
        msg_t msg;
        msg.type = 0;
        msg_send(&msg, pid);
        evtimer_add_msg(&evtimer, &event, pid);
        xtimer_usleep(interval);
    }

    return 0;
}