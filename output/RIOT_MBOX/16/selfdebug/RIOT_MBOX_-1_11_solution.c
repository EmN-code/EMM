#include <stdio.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void message_handler(void) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t pid = thread_getpid();
    printf("Thread PID: %d\n", pid);

    msg_t message;
    message.type = 1;
    msg_send(&message, pid);

    // Starting the message handler
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_WOUT_YIELD,
                  (thread_task_func_t)message_handler, NULL, "msg_handler");

    return 0;
}