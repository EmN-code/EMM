#include <ztimer.h>
#include <msg.h>
#include <event.h>
#include <evtimer.h>

static char stack[THREAD_STACKSIZE_MAIN];
mbox_t my_mbox;
msg_t mbox_queue[8];

static void timer_callback(void *arg) {
    msg_t msg;
    msg.type = *(int *)arg;
    mbox_post(&my_mbox, &msg);
}

int main(void) {
    msg_init_queue(mbox_queue, 8); // Initialize the message queue
    mbox_init(&my_mbox, mbox_queue, 8); // Correctly initialize the mailbox

    ztimer_t timer;
    int message_type = 1;

    // Correctly set up the timer with callback
    timer.callback = timer_callback;
    timer.arg = &message_type;
    ztimer_set(ZTIMER_USEC, &timer, 1000000); // Set timer to trigger every second

    msg_t msg_receive_buffer;
    while(1) {
        if (mbox_get(&my_mbox, &msg_receive_buffer, 1)) {
            printf("Received message of type: %d\n", msg_receive_buffer.type);
        }
    }
    return 0;
}