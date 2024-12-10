#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "architecture.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define STACK_SIZE 2048  // Adjust stack size as required
#define THREAD_PRIORITY_ONE 5
#define THREAD_PRIORITY_TWO 3

static char thread_one_stack[STACK_SIZE];
static char thread_two_stack[STACK_SIZE];

void *thread_one(void *arg) {
    while (1) {
        // Thread one logic here
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Thread two logic here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(thread_one_stack, STACK_SIZE, THREAD_PRIORITY_ONE, 0, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(thread_two_stack, STACK_SIZE, THREAD_PRIORITY_TWO, 0, thread_two, NULL, "thread_two");
    
    // Change priority at runtime
    if (thread_one_pid >= 0 && thread_two_pid >= 0) {
        sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_TWO);
        sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_ONE);
    }
    
    while (1) {
        // Main thread logic, if necessary
    }
    return 0;
}
