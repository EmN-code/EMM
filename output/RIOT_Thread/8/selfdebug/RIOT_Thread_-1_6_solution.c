#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN + 2)

char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *thread_func(void *arg) {
    (void) arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with type: %d and value: %d\n", msg.type, msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    kernel_pid_t second_thread_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_SECOND, 0, 
        thread_func, NULL, "second_thread");

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    msg_send(&msg, second_thread_pid);
    printf("Sent message with type: %d and value: %d\n", msg.type, msg.content.value);

    return 0;
}
