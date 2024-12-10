#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <thread.h>

#define THREAD_STACKSIZE_SMALL 1024

// Function to be executed by thread 1
void *thread_func1(void *arg) {
    (void)arg; // Avoid unused parameter warning
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        // Simulate work
    }
    return NULL;
}

// Function to be executed by thread 2
void *thread_func2(void *arg) {
    (void)arg; // Avoid unused parameter warning
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        // Simulate work
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_SMALL];
    char stack2[THREAD_STACKSIZE_SMALL];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), 10, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), 11, 0, thread_func2, NULL, "thread2");

    // Change the priority of the threads after creation
    thread_set_priority(pid1, 5);
    thread_set_priority(pid2, 6);

    return 0;
}