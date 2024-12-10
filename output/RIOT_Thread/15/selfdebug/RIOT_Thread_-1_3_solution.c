#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN 6
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(&msg, 8);
    printf("Receiver: Ready to receive messages\n");

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with type: %u\n", msg.type);
    }
    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg.type = 1;

    while (1) {
        printf("Sender: Sending message\n");
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_t main_msg_queue[8];
    msg_init_queue(main_msg_queue, 8);

    kernel_pid_t receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE,
                                              THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    thread_create(sender_stack, THREAD_STACKSIZE,
                  THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender, &receiver_pid, "sender");

    return 0;
}