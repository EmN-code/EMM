
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    kernel_pid_t pid;
    char stack[THREAD_STACKSIZE_DEFAULT];

    // Initialize the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Create a thread to check the atomic variable
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, thread_func, NULL, "checker");

    // Disable interrupts and store the counter value
    unsigned state = irq_disable();
    atomic_store(&counter, 0);
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the thread to finish checking
    thread_join(pid);

    // Load the counter value and print it
    int final_count = atomic_load(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}

void *thread_func(void *arg)
{
    (void)arg;
    // Wait for a short period to ensure the interrupt handler has run
    xtimer_sleep(1);

    // Check the counter value
    int current_count = atomic_load(&counter);
    if (current_count == 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return NULL;
}
