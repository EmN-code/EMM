#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)
#define QUEUE_SIZE (8)

static char sender_thread_stack[THREAD_STACKSIZE_MAIN];
static char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

static msg_t queue[QUEUE_SIZE];

kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    msg_init_queue(queue, QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message: %d\n", i);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_RECEIVER, 0, receiver_thread, NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");

    return 0;
}