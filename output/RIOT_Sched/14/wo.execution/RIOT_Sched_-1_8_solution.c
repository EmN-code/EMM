#include <ztimer.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

typedef void *(*thread_task_func_t)(void *arg); 

kernel_pid_t thread_create(char *stack, int stacksize, uint8_t priority, int flags, thread_task_func_t task_func, void *arg, const char *name) {
    // Simulate thread creation and return a dummy thread ID
    static kernel_pid_t next_pid = 1;
    return next_pid++;
}

kernel_pid_t thread_getpid(void) {
    // Simulate getting the current thread ID
    return 0;
}

kernel_pid_t thread_getpid_of(const thread_t *thread) {
    // Simulate getting a thread ID of a specific thread
    return 0;
}

void *thread_func(void *arg) {
    // Thread task simulation
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread2");

    // Simulate changing thread priority
    printf("Thread %d created with priority %d\n", pid1, THREAD_PRIORITY_MAIN);
    printf("Thread %d created with priority %d\n", pid2, THREAD_PRIORITY_MAIN);

    return 0;
}