#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <sched.h>

#define THREAD_PRIORITY_MAIN    (128)
#define SENDER_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN + 2)
#define THREAD_STACKSIZE_DEFAULT  (THREAD_STACKSIZE_DEFAULT)

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        msg.content.value = counter;
        printf("Sending message with content: %d\n", counter);
        msg_send(&msg, thread_getpid_of(receiver_thread));
        ++counter;
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver_thread");

    return 0;
}