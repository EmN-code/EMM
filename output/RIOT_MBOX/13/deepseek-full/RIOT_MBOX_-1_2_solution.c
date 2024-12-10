
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 16

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t mbox_pid;

void event_handler(event_t *event) {
    msg_t msg;
    msg.content.value = 1; // Example event payload
    msg_send(&msg, mbox_pid);
}

void mbox_thread(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }
}

int main(void) {
    mbox_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)mbox_thread, NULL, "mbox");

    evtimer_mbox_t et;
    evtimer_mbox_init(&et, mbox_pid);

    event_t event;
    event.handler = event_handler;

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_mbox_add_msg(&et, &event, NULL);
    timer.target = xtimer_now() + timex_set(1, 0); // 1 second interval

    while (1) {
        xtimer_set(&timer, timex_set(1, 0)); // Reset timer for next event
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
