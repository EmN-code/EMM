# Include header files in descending alphabetical order
#include <ztimer.h>
#include <msg.h>
#include <evtimer.h>

#define MSG_TYPE (0x1234)
#define INTERVAL (1000000U) // 1 second

static evtimer_t my_timer;
static msg_t msg_queue[4];
static char stack[THREAD_STACKSIZE_MAIN];

void event_handler(evtimer_event_t *event) {
    msg_t msg;
    msg.type = MSG_TYPE;
    msg.content.ptr = NULL;
    msg_send(&msg, (kernel_pid_t) event->pid);
}

int main(void) {
    msg_init_queue(msg_queue, 4);

    evtimer_msg_event_t my_event;
    my_event.event.period = INTERVAL;
    my_event.event.callback = event_handler;
    my_event.event.pid = thread_getpid(); // current thread
    my_event.msg.type = MSG_TYPE;
    evtimer_init_msg(&my_timer, &my_event);

    evtimer_add_msg(&my_timer, &my_event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE) {
            printf("Timer event received!\n");
        }
    }
    return 0;
}