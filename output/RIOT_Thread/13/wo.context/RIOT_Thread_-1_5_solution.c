#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE (8)

char sender_thread_stack[STACK_SIZE];
char receiver_thread_stack[STACK_SIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void) arg;

    int pid = thread_getpid();
    printf("Receiver thread started, PID: %d\n", pid);

    while (1) {
        msg_receive(&msg);
        printf("Receiver got message: %d\n", msg.content.value);
        // Simulating work
        xtimer_sleep(1);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void) arg;

    int pid = thread_getpid();
    printf("Sender thread started, PID: %d\n", pid);

    msg_t msg;

    while (1) {
        msg.content.value = 42;
        msg_send(&msg, thread_getpid_of(receiver_thread_stack));
        printf("Sender sent message\n");
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    printf("Initializing threads\n");

    thread_create(receiver_thread_stack, STACK_SIZE,
                  RECEIVER_THREAD_PRIORITY, 0,
                  receiver_thread, NULL, "receiver_thread");

    thread_create(sender_thread_stack, STACK_SIZE,
                  SENDER_THREAD_PRIORITY, 0,
                  sender_thread, NULL, "sender_thread");

    return 0;
}