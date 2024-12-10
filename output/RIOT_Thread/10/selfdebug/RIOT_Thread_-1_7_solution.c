#include <thread.h>
#include <msg.h>
#include <stddef.h>

#define THREAD_PRIORITY_MAIN         (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_DEFAULT     (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_LARGE       (THREAD_STACKSIZE_LARGE)

#define SENDER_PRIO                  (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO                (THREAD_PRIORITY_MAIN)
#define MESSAGE_QUEUE_SIZE           (8)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_LARGE];

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sending message\n");
    msg_send(&msg, thread_getpid_of(thread_get(receiver)));
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                               RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                               receiver, NULL, "receiver");

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                             SENDER_PRIO, THREAD_CREATE_STACKTEST,
                                             sender, NULL, "sender");

    (void)receiver_pid; // to avoid unused variable warning
    (void)sender_pid;

    return 0;
}