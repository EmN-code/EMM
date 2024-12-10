#include "thread_arch.h"
#include "thread_config.h"
#include "stdio.h"
#include "thread.h"

#define STACK_SIZE     (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIO_1  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO_2  (THREAD_PRIORITY_MAIN + 1)

char stack_thread_1[STACK_SIZE];
char stack_thread_2[STACK_SIZE];

void *thread_handler(void *arg) {
    int thread_number = *(int *)arg;
    while (1) {
        printf("Hello from Thread %d\n", thread_number);
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    printf("RIOT ESP32 Threading Example\n");

    int thread_1_num = 1;
    int thread_2_num = 2;

    kernel_pid_t thread_1 = thread_create(stack_thread_1, STACK_SIZE,
                                          THREAD_PRIO_1,
                                          CREATE_STACKTEST,
                                          thread_handler,
                                          &thread_1_num,
                                          "thread_1");

    kernel_pid_t thread_2 = thread_create(stack_thread_2, STACK_SIZE,
                                          THREAD_PRIO_2,
                                          CREATE_STACKTEST,
                                          thread_handler,
                                          &thread_2_num,
                                          "thread_2");

    printf("Threads created with initial priorities\n");

    // Change priorities
    thread_set_priority(thread_1, THREAD_PRIO_2);
    thread_set_priority(thread_2, THREAD_PRIO_1);

    printf("Thread priorities have been switched\n");

    // Infinite main loop
    while (1) {
        // Print status of threads
        printf("Thread 1 status: %d, Priority: %d\n", thread_get_status(thread_getpid_of(&thread_1)), thread_get_priority(thread_getpid_of(&thread_1)));
        printf("Thread 2 status: %d, Priority: %d\n", thread_get_status(thread_getpid_of(&thread_2)), thread_get_priority(thread_getpid_of(&thread_2)));
        thread_yield();
    }

    return 0;
}