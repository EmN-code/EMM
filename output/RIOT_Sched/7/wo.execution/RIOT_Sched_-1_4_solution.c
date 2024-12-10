#include "thread_config.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

void *thread_function(void *arg) {
    while (1) {
        // Thread code
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_function, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_function, NULL, "thread2");

    // Change priority of thread1
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread2
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 2);
    
    return 0;
}