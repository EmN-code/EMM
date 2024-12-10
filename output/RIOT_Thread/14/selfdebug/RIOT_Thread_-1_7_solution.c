#include "ztimer.h"
#include "thread.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
#define WORKER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT

char worker_thread_stack[WORKER_THREAD_STACKSIZE];
msg_t main_msg_queue[MAIN_QUEUE_SIZE];
msg_t worker_msg_queue[MAIN_QUEUE_SIZE];
kernel_pid_t worker_thread;

void *worker_function(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Receive messages
        // Add processing logic here based on received message
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    worker_thread = thread_create(worker_thread_stack, sizeof(worker_thread_stack),
                                  THREAD_PRIORITY_MAIN - 1, 0, worker_function, NULL, "worker");

    msg_t msg;
    msg.type = 1;
    msg_send(&msg, worker_thread);

    // Main thread can also process incoming messages if needed
    while (1) {
        msg_receive(&msg);
        // Add logic to handle messages in main thread, if needed
    }
    return 0;
}