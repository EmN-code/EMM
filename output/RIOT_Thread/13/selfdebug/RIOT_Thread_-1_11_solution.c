#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_function(void *arg) {
    (void)arg; // Unused parameter
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_WOUT_YIELD,
                                            thread_function, NULL, "my_thread");

    msg_t msg;
    msg.content.value = 42;
    msg_send(&msg, thread_pid);
    printf("Sent message with value: %lu\n", (unsigned long)msg.content.value);

    return 0;
}