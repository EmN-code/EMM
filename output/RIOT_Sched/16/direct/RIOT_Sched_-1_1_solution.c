#include <thread.h>
#include <stdio.h>
#include <inttypes.h>
#include <esp_wifi.h>

#define THREAD_STACK_SIZE (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

char thread_stack[THREAD_STACK_SIZE];
char thread_stack2[THREAD_STACK_SIZE];

static void *thread_func(void *arg) {
    (void)arg;
    printf("Thread 1 running with initial priority %u\n", THREAD_PRIORITY_MAIN);
    thread_set_priority(THREAD_PRIORITY_SECOND);
    printf("Thread 1 changed its priority to %u\n", THREAD_PRIORITY_SECOND);
    return NULL;
}

static void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running with initial priority %u\n", THREAD_PRIORITY_SECOND);
    thread_set_priority(THREAD_PRIORITY_MAIN);
    printf("Thread 2 changed its priority to %u\n", THREAD_PRIORITY_MAIN);
    return NULL;
}

int main(void) {
    thread_create(thread_stack, THREAD_STACK_SIZE, THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread_1");
    thread_create(thread_stack2, THREAD_STACK_SIZE, THREAD_PRIORITY_SECOND, 0, thread_func2, NULL, "thread_2");
    
    while (1) {
        printf("Main running\n");
        xtimer_sleep(1);
    }
    return 0;
}