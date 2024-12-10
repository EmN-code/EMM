#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE               THREAD_STACKSIZE_MAIN

char sender_thread_stack[STACK_SIZE];
char receiver_thread_stack[STACK_SIZE];

msg_t msg_queue[8];

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        msg_send(&msg, thread_get_active());
        printf("Sent message: %d\n", msg.content.value);
        thread_yield(); // Yield instead of thread_sleep
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, STACK_SIZE,
                                              RECEIVER_THREAD_PRIORITY, CREATE_WOUT_YIELD,
                                              receiver, NULL, "receiver");
    
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, STACK_SIZE,
                                            SENDER_THREAD_PRIORITY, CREATE_WOUT_YIELD,
                                            sender, NULL, "sender");

    if (sender_pid > 0 && receiver_pid > 0) {
        printf("Threads created successfully\n");
    } else {
        printf("Error creating threads\n");
    }

    return 0;
}