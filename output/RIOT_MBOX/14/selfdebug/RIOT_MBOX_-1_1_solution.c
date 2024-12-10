#include <ztimer.h>
#include <thread.h>
#include <msg.h>

#define INTERVAL   (1U * ZTIMER_MSEC) // Timer interval of 1 second
#define MSG_QUEUE_SIZE   (8)

static kernel_pid_t my_event_thread_pid;
static char my_event_thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t my_msg_queue[MSG_QUEUE_SIZE];

static void *_my_event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received an event notification\n");
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(my_msg_queue, MSG_QUEUE_SIZE);

    // Create the event handling thread
    my_event_thread_pid = thread_create(my_event_thread_stack, sizeof(my_event_thread_stack),
                                        THREAD_PRIORITY_MAIN - 1, 0,
                                        _my_event_thread, NULL, "my_event_thread");

    // Initialize timer
    ztimer_t timer;
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg_my_event, my_event_thread_pid);

    // Simulate sending message at intervals
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, INTERVAL);
        printf("Sending an event\n");
        msg_t msg = { .type = 0 };
        msg_send(&msg, my_event_thread_pid);
    }
    return 0;
}