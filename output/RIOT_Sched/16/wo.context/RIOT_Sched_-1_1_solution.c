#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define STACK_SIZE_THREAD   (1024)
#define THREAD_PRIORITY_1   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2   (THREAD_PRIORITY_MAIN - 2)

char stack1[STACK_SIZE_THREAD];
char stack2[STACK_SIZE_THREAD];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void* thread1_entry(void* arg) {
    (void)arg;
    printf("Thread 1 running\n");
    thread_sleep();
    return NULL;
}

void* thread2_entry(void* arg) {
    (void)arg;
    printf("Thread 2 running\n");
    thread_sleep();
    return NULL;
}

int main(void) {
    puts("RIOT multi-threading example: starting threads\n");

    // Start thread 1
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0,
                                thread1_entry, NULL, "thread1");
    
    // Start thread 2
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0,
                                thread2_entry, NULL, "thread2");

    // Change the priority of thread 1
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_2);

    // Change the priority of thread 2
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_1);

    puts("Threads started and priorities changed\n");

    return 0;
}