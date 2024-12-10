#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define DELAY (ZTIMER_MSEC(1000))
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, DELAY);
        msg.content.value = 42;
        printf("Sender: sending msg with value %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                    THREAD_PRIORITY_MAIN - 1, 0,
                    sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received msg with value %d\n", msg.content.value);
    }
    return 0;
}