#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t main_pid = sched_active_thread->pid;
    
    evtimer_init_msg(&evtimer);
    msg_send_receive(&msg, &msg, main_pid);
    evtimer_add_msg(&evtimer, &event, main_pid);

    while (1) {
        xtimer_sleep(3);
        msg.content.value = 42; // Example message content
        msg_send(&msg, main_pid);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Process the received message
printf("Received message with value: %ld\n", msg.content.value);
    }

    return 0;
}