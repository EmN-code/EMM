#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <thread.h>

#define STACK_SIZE 1024

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread1_handler(void *arg) {
    // Exemplary task
    while(1) {
        // Simulated task and priority change
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    // Exemplary task
    while(1) {
        // Simulated task and priority change
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), 1, 0,
                                       thread1_handler, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), 2, 0,
                                       thread2_handler, NULL, "thread2");

    // Simulate priority change
    // In actual implementation, you'd call a function to change priorities here
    return 0;
}