#include <thread_arch.h>
#include <pthread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (30)

void *thread_func_one(void *arg) {
    printf("Thread One running!\n");
    return NULL;
}

void *thread_func_two(void *arg) {
    printf("Thread Two running!\n");
    return NULL;
}

int main(void) {
    pthread_t thread_one, thread_two;
    pthread_attr_t thread_attr_one, thread_attr_two;
    struct sched_param param_one, param_two;

    // Initialize attributes
    pthread_attr_init(&thread_attr_one);
    pthread_attr_init(&thread_attr_two);

    // Set stack size
    pthread_attr_setstacksize(&thread_attr_one, THREAD_STACKSIZE_MAIN);
    pthread_attr_setstacksize(&thread_attr_two, THREAD_STACKSIZE_MAIN);

    // Set initial priorities
    param_one.sched_priority = THREAD_PRIORITY_MAIN;
    param_two.sched_priority = THREAD_PRIORITY_MAIN - 2;
    pthread_attr_setschedparam(&thread_attr_one, &param_one);
    pthread_attr_setschedparam(&thread_attr_two, &param_two);

    // Start threads
    pthread_create(&thread_one, &thread_attr_one, thread_func_one, NULL);
    pthread_create(&thread_two, &thread_attr_two, thread_func_two, NULL);

    // Change priorities at runtime
    param_one.sched_priority = THREAD_PRIORITY_MAIN - 1;
    param_two.sched_priority = THREAD_PRIORITY_MAIN;
    pthread_setschedparam(thread_one, SCHED_FIFO, &param_one);
    pthread_setschedparam(thread_two, SCHED_FIFO, &param_two);

    // Join threads
    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    return 0;
}