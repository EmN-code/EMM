#include <xtimer.h>
#include <msg.h>
#include <kernel.h>
#include <event/thread.h>
#include <event.h>

#define TIMER_INTERVAL (1000000U)  // 1 second interval
#define QUEUE_SIZE (8)

static msg_t queue[QUEUE_SIZE];
static msg_t msg;
static kernel_pid_t main_pid;

static void timer_callback(void *arg) {
    (void)arg;
    msg_send(&msg, main_pid);
}

int main(void) {
    main_pid = thread_getpid();
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_receive(&msg);
        // Handle received message
    }

    return 0;
}