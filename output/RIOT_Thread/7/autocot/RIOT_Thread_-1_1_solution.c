#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIORITY       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY     (THREAD_PRIORITY_MAIN - 2)
#define MESSAGE_QUEUE_SIZE    (8)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // some arbitrary data
    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIORITY, 0, receiver_thread,
                                 NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIORITY, 0, sender_thread,
                  NULL, "sender");

    return 0;
}