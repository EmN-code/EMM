#include <thread.h>
#include <stdio.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_DEFAULT        (THREAD_STACKSIZE_MAIN)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler1(void *arg) {
    (void) arg;
    printf("Thread 1 starting\n");
    while (1) {
        xtimer_sleep(2);
        printf("Thread 1 executing\n");
    }
    return NULL;
}

void *thread_handler2(void *arg) {
    (void) arg;
    printf("Thread 2 starting\n");
    while (1) {
        xtimer_sleep(3);
        printf("Thread 2 executing\n");
    }
    return NULL;
}

int main(void) {
    thread_t thread1;
    thread_t thread2;

    printf("Main thread starting\n");

    /* Create Thread 1 */
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_handler1, NULL, "thread1");
    
    /* Create Thread 2 */
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_handler2, NULL, "thread2");

    /* Sleep for 10 seconds */
    xtimer_sleep(10);

    /* change priority of Thread 1 */
    printf("Changing priority of Thread 1\n");
    thread_set_priority(thread1, THREAD_PRIORITY_MAIN - 3);

    /* Sleep for another 10 seconds */
    xtimer_sleep(10);

    /* change priority of Thread 2 */
    printf("Changing priority of Thread 2\n");
    thread_set_priority(thread2, THREAD_PRIORITY_MAIN - 4);

    /* Let threads run indefinitely */
    while(1) {}

    return 0;
}
