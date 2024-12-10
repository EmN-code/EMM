#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_buffer[8];

void *receiver_thread_func(void *arg) {
    (void)arg;
    msg_init_queue(msg_buffer, 8);
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread_func(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    while (1) {
        msg_t msg;
        msg.content.value = 42; // Example value
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN, 0,
                                              receiver_thread_func, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread_func, &receiver_pid, "sender");

    return 0;
}