#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

// Thread stacks
char stack_thread_1[THREAD_STACKSIZE_MAIN];
char stack_thread_2[THREAD_STACKSIZE_MAIN];

// Thread PIDs
kernel_pid_t thread_1_pid;
kernel_pid_t thread_2_pid;

void *thread_1_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 1 is running...");
        thread_yield_higher();
    }
    return NULL;
}

void *thread_2_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 2 is running...");
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    // Initialize UART for output
    uart_rxstart_irq_enable(uart_default);  // Enable UART for printf
    
    // Start thread 1
    thread_1_pid = thread_create(stack_thread_1, sizeof(stack_thread_1),
                                 THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                 thread_1_handler, NULL, "thread_1");

    // Start thread 2
    thread_2_pid = thread_create(stack_thread_2, sizeof(stack_thread_2),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 thread_2_handler, NULL, "thread_2");

    // Simulating runtime priority change
    puts("Changing priority of threads at runtime");
    sched_change_priority(thread_1_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_2_pid, THREAD_PRIORITY_MAIN - 3);

    while (1) {
        // Main thread work
        puts("Main is running...");
        thread_yield_higher();
    }
    return 0;
}