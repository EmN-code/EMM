#include "thread.h"
#include "event/thread.h"

#define THREAD_STACKSIZE 1024  // defining stack size for threads

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_func(void *arg) {
    (void)arg; // suppress unused parameter warning
    while (1) {
        // Thread functionality
    }
    return NULL;
}

int main(void) {
    thread1 = thread_create(stack1, sizeof(stack1), 5, 0, thread_func, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), 6, 0, thread_func, NULL, "thread2");

    // Assume here is logic to change priority at runtime if needed
    
    return 0;
}