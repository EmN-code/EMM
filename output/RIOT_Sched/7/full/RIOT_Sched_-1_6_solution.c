#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN  (1024 * 4)
#define THREAD_PRIORITY_MAIN   (0)

pthread_t thread1, thread2;

void *thread1_function(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_function(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main() {
    // Create thread 1
    if (pthread_create(&thread1, NULL, thread1_function, NULL)) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    // Create thread 2
    if (pthread_create(&thread2, NULL, thread2_function, NULL)) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    // Change priority of threads (demonstration purposes as setting priority requires platform-specific extensions)
    struct sched_param param;
    param.sched_priority = 1; // Higher number lower priority, adjust as needed
    pthread_setschedparam(thread1, SCHED_RR, &param);
    param.sched_priority = 2; // Lower priority than thread1
    pthread_setschedparam(thread2, SCHED_RR, &param);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}