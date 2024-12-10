#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_OTHER (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_OTHER (THREAD_PRIORITY_MAIN + 1)
#define PRIORITY_CHANGED (THREAD_PRIORITY_MAIN - 3)

char stack_thread1[THREAD_STACKSIZE_OTHER];
char stack_thread2[THREAD_STACKSIZE_OTHER];

static kernel_pid_t thread1_pid;
static kernel_pid_t thread2_pid;

void *thread_handler(void *arg) {
    printf("Thread %d running with priority %d\n", thread_getpid(), thread_get_priority());
    return NULL;
}

void *thread_changer(void *arg) {
    (void)arg;
    thread_yield_higher();
    printf("Changing priority for thread 1\n");
    thread_set_priority(thread1_pid, PRIORITY_CHANGED);
    printf("Thread 1 new priority: %d\n", PRIORITY_CHANGED);
    return NULL;
}

int main(void) {
    puts("RIOT priority test program\n");

    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1), PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2), PRIORITY_OTHER, THREAD_CREATE_STACKTEST, thread_changer, NULL, "thread2");
    
    printf("Main thread PID: %d\n", thread_getpid());

    // Let threads run
    thread_yield();

    return 0;
}