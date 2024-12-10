#include <thread_arch.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (7)

static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Thread routine here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;

    // Start the first thread
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                               CREATE_STACKTEST, thread_handler, NULL, "thread1");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        return 1; // Error: Thread creation failed
    }

    // Change priority of the started thread
    thread_set_priority(thread_pid, THREAD_PRIORITY_MAIN - 2);

    // Start another thread with a different priority
    static char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 3,
                                             CREATE_STACKTEST, thread_handler, NULL, "thread2");

    if (thread_pid2 <= KERNEL_PID_UNDEF) {
        return 1; // Error: Thread creation failed
    }

    // Change priority of the second thread
    thread_set_priority(thread_pid2, THREAD_PRIORITY_MAIN);

    while (1) {
        // Main routine here
    }
    return 0;
}