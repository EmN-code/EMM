#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (2048)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 2)
#define THREAD_SLEEP_SECONDS     (5)
#define THREAD_NAME_ONE          "thread_one"
#define THREAD_NAME_TWO          "thread_two"

typedef struct {
    char name[10];
    uint8_t priority;
    char stack[THREAD_STACKSIZE_MAIN];
} my_thread_t;

void *thread_work(void *arg) {
    my_thread_t *my_thread = (my_thread_t *)arg;
    printf("%s running at priority %d\n", my_thread->name, my_thread->priority);
    for (;;) {
        puts(my_thread->name);
    }
    return NULL;
}

int main(void) {
    my_thread_t thread_one;
    my_thread_t thread_two;
    thread_one.priority = THREAD_PRIORITY_MAIN;
    snprintf(thread_one.name, sizeof(thread_one.name), THREAD_NAME_ONE);
    thread_two.priority = THREAD_PRIORITY_MAIN;
    snprintf(thread_two.name, sizeof(thread_two.name), THREAD_NAME_TWO);

    thread_create(thread_one.stack, sizeof(thread_one.stack), thread_one.priority,
                  0, thread_work, &thread_one, thread_one.name);

    thread_create(thread_two.stack, sizeof(thread_two.stack), thread_two.priority,
                  0, thread_work, &thread_two, thread_two.name);

    puts("Both threads are started");

    xtimer_sleep(THREAD_SLEEP_SECONDS);

    printf("Changing priority of %s\n", THREAD_NAME_ONE);
    thread_one.priority = THREAD_PRIORITY_MAIN - 1;
    sched_change_priority(thread_getpid_of((const thread_t*)&thread_one), thread_one.priority);

    return 0;
}