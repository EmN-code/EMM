#include <thread.h>
#include <stdio.h>
#include <msg.h>

char thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    while (1) {
        msg.content.value = rand() % 100;
        msg_send(&msg, receiver_pid);
        printf("Sent message: %ld\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL, "receiver");

    if (receiver_pid < 0) {
        printf("Error creating receiver thread\n");
    }
    else {
        char sender_stack[THREAD_STACKSIZE_MAIN];
        thread_create(
            sender_stack, sizeof(sender_stack),
            THREAD_PRIORITY_MAIN + 1,
            THREAD_CREATE_STACKTEST,
            sender_thread,
            &receiver_pid, "sender");
    }
    return 0;
}