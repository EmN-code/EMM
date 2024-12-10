#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2     (THREAD_PRIORITY_MAIN - 2)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread 1 running with priority: %d\n", thread_get_priority(thread_getpid()));
    
    xtimer_sleep(2);
    
    printf("Thread 1 changing priority\n");
    thread_set_priority(thread_getpid(), THREAD_PRIORITY_2);
    printf("Thread 1 new priority: %d\n", thread_get_priority(thread_getpid()));
    
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread 2 running with priority: %d\n", thread_get_priority(thread_getpid()));
    
    xtimer_sleep(1);
    
    printf("Thread 2 changing priority\n");
    thread_set_priority(thread_getpid(), THREAD_PRIORITY_MAIN);
    printf("Thread 2 new priority: %d\n", thread_get_priority(thread_getpid()));
    
    return NULL;
}

int main(void) {
    thread_create(thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN, 0,
                  thread_one, NULL, "thread_one");
                  
    thread_create(thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_2, 0,
                  thread_two, NULL, "thread_two");
    
    return 0;
}