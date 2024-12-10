#include <thread_arch.h>
#include <thread.h>
#include <sched.h>
#include <event/thread.h>
#include <architecture.h>

#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_OTHER 7
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_STACKSIZE_OTHER 1024

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_OTHER];

void *thread_handler(void *arg) {
    int num = *((int *)arg);
    while (1) {
        printf("Thread %d running\n", num);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;
    int arg1 = 1, arg2 = 2;

    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                            CREATE_STACKTEST, thread_handler, &arg1, "thread1");

    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_OTHER,
                            CREATE_STACKTEST, thread_handler, &arg2, "thread2");

    printf("Threads created\n");
    
    // Changing the priority of thread1 and thread2 at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_OTHER);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN);

    while (1) {
        printf("Main thread running\n");
        thread_yield();
    }
    return 0;
}