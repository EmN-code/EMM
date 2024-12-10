#include <thread_arch.h>
#include <thread.h>
#include <sched.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <architecture.h>

void *thread_func1(void *arg) {
    printf("Thread 1 is running\n");
    while(1) {
        printf("Thread 1 is working\n");
        ztimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running\n");
    while(1) {
        printf("Thread 2 is working\n");
        ztimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread1, thread2;

    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Explanation: Initial thread creation
    while (1) {
        kernel_pid_t active_thread = thread_getpid();
        printf("Active thread PID: %d\n", active_thread);

        // Change priority logic
        sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
        sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

        // Network connection retry example
        int rc;
        while (1) {
            rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
            if (rc != 0) {
                printf("Network connection failed. Retrying...\n");
                ztimer_sleep(1);
                continue;
            } else {
                printf("Network connected successfully.\n");
                break;
            }
        }

        ztimer_sleep(5);
    }

    return 0;
}