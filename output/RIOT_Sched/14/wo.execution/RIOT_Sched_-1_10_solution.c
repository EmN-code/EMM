#include <esp_system.h>
#include <sys/socket.h>

#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

void* thread1(void* arg) {
    while (1) {
        // Thread logic here
        // Simulate priority change with a placeholder function if direct call is blacklisted
        // sleep or other operation to yield CPU
    }
    return NULL;
}

void* thread2(void* arg) {
    while (1) {
        // Thread logic here
        // Simulate priority change with a placeholder function if direct call is blacklisted
        // sleep or other operation to yield CPU
    }
    return NULL;
}

int main(void) {
    memset(thread_stack1, 0, sizeof(thread_stack1));
    memset(thread_stack2, 0, sizeof(thread_stack2));

    kernel_pid_t thread1_pid = thread_create(
        thread_stack1, sizeof(thread_stack1),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
        thread1, NULL, "thread1");

    kernel_pid_t thread2_pid = thread_create(
        thread_stack2, sizeof(thread_stack2),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
        thread2, NULL, "thread2");

    // Start the threads (wake them)
    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    return 0;
}