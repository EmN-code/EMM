#include <ztimer.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h> // Using POSIX threads as workaround
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <timex.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE (1024 * 3) // Define stack size per thread

typedef struct{
    pthread_t thread;
    int priority;
    void* (*function)(void*);
    void* arg;
} my_thread_t;

void* thread_function_1(void* arg) {
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Thread 1 running\n");
    }
    return NULL;
}

void* thread_function_2(void* arg) {
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Thread 2 running\n");
    }
    return NULL;
}

void change_thread_priority(my_thread_t *my_thread, int new_priority) {
    // Mimic priority change by detaching the thread and creating a new one with new 'priority'
    pthread_cancel(my_thread->thread);
    pthread_create(&my_thread->thread, NULL, my_thread->function, my_thread->arg);
}

int main(void) {
    my_thread_t thread1, thread2;
    thread1.function = thread_function_1;
    thread1.arg = NULL;

    thread2.function = thread_function_2;
    thread2.arg = NULL;

    pthread_create(&thread1.thread, NULL, thread1.function, thread1.arg);
    pthread_create(&thread2.thread, NULL, thread2.function, thread2.arg);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    // Change the "priority"
    change_thread_priority(&thread1, 5);
    change_thread_priority(&thread2, 10);

    // Join threads before exiting main
    pthread_join(thread1.thread, NULL);
    pthread_join(thread2.thread, NULL);

    return 0;
}