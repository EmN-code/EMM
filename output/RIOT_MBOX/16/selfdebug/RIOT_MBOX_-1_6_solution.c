#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "event/callback.h"
#include "xtimer.h"
#include "evtimer.h"
#include "kernel.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t event_pid;
static evtimer_t event_timer;
static evtimer_msg_event_t event;

void event_timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;    // some message type identifier
    msg_send(&msg, event_pid);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char stack[KERNEL_CONF_STACKSIZE_DEFAULT];
    event_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                              event_timer_callback, NULL, "event thread");

    evtimer_init_msg(&event_timer, &event, event_pid, 0);
    evtimer_add(&event_timer, xtimer_usec_from_sec(2));

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        // Handle received message
        if (msg.type == 0x1234) {
            puts("Received event message!");
        }
    }

    return 0;
}