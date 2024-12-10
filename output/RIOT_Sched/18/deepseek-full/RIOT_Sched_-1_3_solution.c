
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "unistd.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 3

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

static void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(5);
    }
    return NULL;
}

static void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_LOW, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priorities\n");
    thread_set_priority(pid1, PRIORITY_HIGH);
    thread_set_priority(pid2, PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priorities again\n");
    thread_set_priority(pid1, PRIORITY_LOW);
    thread_set_priority(pid2, PRIORITY_HIGH);

    return 0;
}
