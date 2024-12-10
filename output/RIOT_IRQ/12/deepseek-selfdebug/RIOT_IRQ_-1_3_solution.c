
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static kernel_pid_t sender_pid;
static atomic_int counter = ATOMIC_VAR_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        atomic_fetch_add(&counter, 1);
        printf("Counter: %d\n", atomic_load(&counter));
        thread_yield();
    }

    return NULL;
}

int main(void)
{
    sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_func, NULL, "counter");

    if (sender_pid <= 0) {
        puts("Error creating thread");
        return 1;
    }

    while (1) {
        unsigned state = irq_disable();
        atomic_fetch_add(&counter, 1);
        irq_restore(state);
        printf("Main Counter: %d\n", atomic_load(&counter));
        thread_sleep(1000000);
    }

    return 0;
}
