#include <stdio.h>
#include <stdint.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO     (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

msg_t msg_queue[8];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;

    for (uint32_t i = 0; i < 5; i++) {
        msg.content.value = i;
        printf("Sender: Sending message with value %lu\n", (unsigned long)msg.content.value);
        msg_send(&msg, thread_getpid_of_tname("Receiver"));
    }
    return NULL;
}

static void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    for (uint32_t i = 0; i < 5; i++) {
        msg_t msg;
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_PRIO, 0, sender, NULL, "Sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_PRIO, 0, receiver, NULL, "Receiver");

    printf("Sender thread PID: %d\n", sender_pid);
    printf("Receiver thread PID: %d\n", receiver_pid);

    return 0;
}