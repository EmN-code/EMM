#include <ztimer.h>
#include <msg.h>
#include <stdio.h>
#include <thread.h>

#define MAIN_QUEUE_SIZE (8)
#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char sender_stack[SENDER_STACKSIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg.content.value = 42; // Example message content
        printf("Sender: Posting message with content %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(sender_stack, SENDER_STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread: Received message with content %d\n", msg.content.value);
    }
    return 0;
}