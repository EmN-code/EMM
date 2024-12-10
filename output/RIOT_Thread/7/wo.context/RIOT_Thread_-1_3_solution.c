#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

#define QUEUE_SIZE               (8)  // Power of two size
static msg_t _msg_queue[QUEUE_SIZE];  // A queue of messages

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // example message content
    while (1) {
        printf("Sending message...\n");
        msg_send(&msg, thread_getpid_of(receiver_thread));
        xtimer_sleep(1);  // Ensure xtimer_sleep is declared
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(_msg_queue, QUEUE_SIZE);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                        SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                        sender_thread, NULL, "sender_thread");

    kernel_pid_t receiver = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                          RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                          receiver_thread, NULL, "receiver_thread");
    if (sender <= KERNEL_PID_UNDEF || receiver <= KERNEL_PID_UNDEF) {
        printf("Failed to start threads");
        return 1;
    }
    printf("Threads successfully created\n");
    return 0;
}