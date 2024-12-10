#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <thread_config.h>
// Note: MQTTClient.h and paho_mqtt.h are part of external libraries and can't be included unless they define needed types

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5 
#define THREAD_PRIORITY_AUX 6

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Thread tasks here
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                thread_handler, NULL, "thread1");
    
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_AUX, 0,
                                thread_handler, NULL, "thread2");

    // Change the priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_AUX);

    // Change the priority of thread2 at runtime
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    // Main thread loop
    while (1) {
        // Operations in the main thread
    }
    return 0;
}