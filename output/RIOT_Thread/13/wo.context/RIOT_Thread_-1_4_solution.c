#include <thread.h>
#include <msg.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_PRIORITY_SENDER    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER  (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE          THREAD_STACKSIZE_DEFAULT

static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        printf("Sender: Sending message with value %lu\n", msg.content.value);
        msg_send(&msg, thread_getpid_by_name("receiver"));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    
    printf("Sender thread PID: %d\n", sender_pid);
    printf("Receiver thread PID: %d\n", receiver_pid);
    
    return 0;
}
