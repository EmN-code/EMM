#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"
#include "architecture.h"

#define STACKSIZE_THREAD_ONE  (1024)
#define STACKSIZE_THREAD_TWO  (1024)
#define PRIORITY_THREAD_ONE   (3)
#define PRIORITY_THREAD_TWO   (2)

char stack_thread_one[STACKSIZE_THREAD_ONE];
char stack_thread_two[STACKSIZE_THREAD_TWO];

void *thread_task_one(void *arg) {
    while (1) {
        // Perform some operations in thread one
    }
    return NULL;
}

void *thread_task_two(void *arg) {
    while (1) {
        // Perform some operations in thread two
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;

    thread_one_pid = thread_create(stack_thread_one, STACKSIZE_THREAD_ONE, PRIORITY_THREAD_ONE, 0, thread_task_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack_thread_two, STACKSIZE_THREAD_TWO, PRIORITY_THREAD_TWO, 0, thread_task_two, NULL, "thread_two");

    // Change priority of the thread at runtime; here we swap priorities of two threads
    thread_set_priority(thread_one_pid, PRIORITY_THREAD_TWO);
    thread_set_priority(thread_two_pid, PRIORITY_THREAD_ONE);

    return 0;
}