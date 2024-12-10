
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACK_SIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 10

static char stack[STACK_SIZE];
static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void event_sender(void) {
    msg_t msg;
    msg.content.value = 1; // Example event value

    while (1) {
        xtimer_sleep(5); // Send event every 5 seconds
        msg_send(&msg, thread_getpid());
    }
}

void event_receiver(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)event_sender, NULL, "event_sender");
    thread_create(stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)event_receiver, NULL, "event_receiver");

    return 0;
}
