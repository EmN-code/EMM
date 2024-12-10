#include <thread.h>
#include <xtimer.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)

static char sender_thread_stack[THREAD_STACKSIZE_MAIN];
static char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[MSG_QUEUE_SIZE];
kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 12345; // example payload
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sender: Sent message with value %d\n", msg.content.value);
        xtimer_sleep(1);  // send every second
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_RECEIVER, 0, receiver, NULL, "receiver");
    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_SENDER, 0, sender, NULL, "sender");
    return 0;
}