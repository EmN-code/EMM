#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "pthread.h"
#include "kernel_defines.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "sched.h"
#include "thread.h"
#include "event/thread.h"
#include "time_units.h"

#define UNUSED(x) (void)(x)
#define MQTT_BROKER_IP   "192.168.1.1"
#define MQTT_BROKER_PORT 1883

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    UNUSED(arg);
    printf("Thread is running\n");
    return NULL;
}

void change_thread_priority(pthread_t thread, int new_priority) {
    // Assume sched_setscheduler is properly defined elsewhere
    struct sched_param param;
    param.sched_priority = new_priority;
    pthread_setschedparam(thread, SCHED_FIFO, &param);
}

int main(void) {
    pthread_t thread1, thread2;

    // Create thread1
    if (pthread_create(&thread1, NULL, thread_function, NULL) != 0) {
        printf("Thread1 creation failed\n");
        return 1;
    }
    printf("Thread1 created successfully\n");

    // Create thread2
    if (pthread_create(&thread2, NULL, thread_function, NULL) != 0) {
        printf("Thread2 creation failed\n");
        return 1;
    }
    printf("Thread2 created successfully\n");

    // Change priority of thread1
    change_thread_priority(thread1, 2);
    printf("Changed priority of Thread1\n");

    // Network connection with retry logic
    while (1) {
        int rc = NetworkConnect(NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            sleep(1); // Retry every 1 second
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    // Wait for the threads to finish execution
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}