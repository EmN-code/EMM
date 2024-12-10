#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_function(void* arg) {
    int id = *((int*)arg);
    printf("Thread %d is running with priority %d\n", id, sched_get_priority_min(SCHED_FIFO));
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    struct sched_param param;
    int priority;
    int id1 = 1, id2 = 2;

    // Create thread 1
    if (pthread_create(&thread1, NULL, thread_function, &id1)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Set a higher priority for thread 1
    priority = sched_get_priority_max(SCHED_FIFO);
    param.sched_priority = priority;
    if (pthread_setschedparam(thread1, SCHED_FIFO, &param)) {
        fprintf(stderr, "Error setting thread priority\n");
        return 1;
    }
    printf("Thread 1 priority set to %d\n", priority);

    // Create thread 2
    if (pthread_create(&thread2, NULL, thread_function, &id2)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Set a lower priority for thread 2
    priority = sched_get_priority_min(SCHED_FIFO);
    param.sched_priority = priority;
    if (pthread_setschedparam(thread2, SCHED_FIFO, &param)) {
        fprintf(stderr, "Error setting thread priority\n");
        return 1;
    }
    printf("Thread 2 priority set to %d\n", priority);

    // Wait for threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}