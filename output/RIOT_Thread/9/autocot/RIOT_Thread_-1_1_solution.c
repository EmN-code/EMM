#include <xtimer.h> 
#include "msg.h"
#include "thread.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static char thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *thread_function(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    while (1) {
        // Receive message
        msg_receive(&msg);
        puts("Received message!");
    }

    return NULL;
}

int main(void) {
    msg_t msg;
    kernel_pid_t thread_pid;

    // Create new thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_function,
                               NULL, "receiver");

    // Send a message from main thread to the newly created thread
    msg.content.value = 42;
    msg_send(&msg, thread_pid);

    puts("Message sent!");

    return 0;
}