#include <thread_config.h>
#include <stdlib.h>
#include <stddef.h>
#include <msg.h>
#include <event/thread.h>

#define EVENT_TASK_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)

static char event_task_stack[EVENT_TASK_STACKSIZE];

event_queue_t event_queue;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        msg_send(&msg, thread_get(EVENT_PRIO_MEDIUM));
        xtimer_sleep(1);
    }
}

int main(void) {
    event_queue_init(&event_queue);
    thread_create(event_task_stack, sizeof(event_task_stack), EVENT_PRIO_MEDIUM,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                  sender_thread, NULL, "sender_thread");
    return 0;
}