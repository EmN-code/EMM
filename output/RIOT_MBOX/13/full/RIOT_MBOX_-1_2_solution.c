#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define INTERVAL (1U * US_PER_SEC)
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN PRIORITY_MAIN

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *event_thread(void *arg) {
    (void)arg;

    msg_t msg;
    ztimer_t timer;

    // Initialize timer for periodic events
    ztimer_set_msg(ZTIMER_MBOX, &timer, INTERVAL, thread_getpid(), &msg);

    while (1) {
        ztimer_read_block(ZTIMER_MBOX, &timer);
        printf("Event triggered\n");
    }

    return NULL;
}

int main(void) {
    printf("Starting message box example\n");

    // Create a thread to send events
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN + 1,
                  0, event_thread, NULL, "event_thread");

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: Type %u\n", msg.type);
    }

    return 0;
}
