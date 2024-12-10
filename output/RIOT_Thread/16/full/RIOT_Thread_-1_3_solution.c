#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <mutex.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MAIN_QUEUE_SIZE (8)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static msg_t msg;

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1111;

    while (1) {
        printf("Sending message\n");
        msg_send(&msg, kernel_pid_t receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

static void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message\n");
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0,
                                          sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, 0,
                                          receiver, NULL, "receiver");

    return 0;
}