#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(2);  // Sleep for 2 seconds
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, thread_handler,
                                            NULL, "thread_handler");

    (void)sender_pid; // Since sender_pid is not used, suppress unused variable warning
    
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Process the message (currently, we do nothing with it)
    }
    return 0;
}