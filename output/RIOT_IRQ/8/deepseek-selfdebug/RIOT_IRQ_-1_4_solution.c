
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

/* Define an atomic variable */
atomic_int atomic_var = ATOMIC_INIT(0);

/* Function to be executed by the thread */
void *thread_func(void *arg)
{
    (void)arg;

    /* Disable interrupts */
    unsigned state = irq_disable();

    /* Modify the atomic variable */
    atomic_store(&atomic_var, 1);

    /* Restore interrupts */
    irq_restore(state);

    return NULL;
}

int main(void)
{
    puts("Starting interrupt handling test...");

    /* Create a thread */
    kernel_pid_t pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_func, NULL, "thread");

    /* Wait for the thread to finish */
    thread_yield();

    /* Check the value of the atomic variable */
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        puts("Interrupts were disabled and restored correctly.");
    } else {
        puts("Interrupts were not handled correctly.");
    }

    return 0;
}
