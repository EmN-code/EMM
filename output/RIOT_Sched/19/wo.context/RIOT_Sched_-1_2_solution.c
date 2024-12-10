#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (4096)
#define THREAD_CREATE_STACKTEST THREAD_CREATE_WOUT_YIELD
#define MY_PRIORITY             (THREAD_PRIORITY_MAIN - 1)

char stack_thread_a[THREAD_STACKSIZE_MAIN];
char stack_thread_b[THREAD_STACKSIZE_MAIN];
kernel_pid_t pid_thread_a;
kernel_pid_t pid_thread_b;

void *thread_a(void *arg) {
    printf("Thread A started\n");
    return NULL;
}

void *thread_b(void *arg) {
    printf("Thread B started\n");
    return NULL;
}

int main(void) {
    printf("Main thread started\n");

    pid_thread_a = thread_create(stack_thread_a, sizeof(stack_thread_a),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 thread_a, NULL, "thread_a");

    pid_thread_b = thread_create(stack_thread_b, sizeof(stack_thread_b),
                                 THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                 thread_b, NULL, "thread_b");

    if (pid_thread_a <= KERNEL_PID_UNDEF || pid_thread_b <= KERNEL_PID_UNDEF) {
        printf("Error creating threads\n");
        return 1;
    }

    printf("Threads created. Changing priorities...\n");
    sched_change_priority(thread_get(pid_thread_a), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_thread_b), THREAD_PRIORITY_MAIN - 1);

    printf("Priorities changed.\n");
    return 0;
}