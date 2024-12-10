#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <thread.h>

#define THREAD_PRIORITY_MAIN  (7U) // Defining main thread priority
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    printf("Hello from the new thread!\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid;

    puts("Starting threading example...");

    // Create a new thread
    pid = thread_create(stack, sizeof(stack),
                        THREAD_PRIORITY_MAIN, 0,
                        thread_handler, NULL, "thread_handler");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create thread.");
        return 1;
    }
    
    // Wait for the thread to finish its execution
    thread_yield();

    return 0;
}