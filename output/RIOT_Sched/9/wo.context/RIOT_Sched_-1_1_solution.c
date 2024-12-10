#include <thread_arch.h>
#include <thread.h>  

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_A_PRIORITY       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_B_PRIORITY       (THREAD_PRIORITY_MAIN - 2)

static kernel_pid_t thread_a_pid;
static kernel_pid_t thread_b_pid;
static char thread_a_stack[THREAD_STACKSIZE_MAIN];
static char thread_b_stack[THREAD_STACKSIZE_MAIN];

void *thread_a(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread A running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread B running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    puts("Starting two threads on ESP32\n");

    // Create thread A
    thread_a_pid = thread_create(thread_a_stack, sizeof(thread_a_stack),
                                 THREAD_A_PRIORITY, THREAD_CREATE_STACKTEST,
                                 thread_a, NULL, "thread_a");

    // Create thread B
    thread_b_pid = thread_create(thread_b_stack, sizeof(thread_b_stack),
                                 THREAD_B_PRIORITY, THREAD_CREATE_STACKTEST,
                                 thread_b, NULL, "thread_b");

    // Simulate changing priorities at runtime
    puts("Changing priorities\n");
    sched_change_priority(thread_a_pid, THREAD_B_PRIORITY);
    sched_change_priority(thread_b_pid, THREAD_A_PRIORITY);

    while (1) {
        /* Thread management happens automatically in RIOT 
         * Let the main thread sleep to simulate idle behavior 
         */
        thread_yield();
    }

    return 0;
}