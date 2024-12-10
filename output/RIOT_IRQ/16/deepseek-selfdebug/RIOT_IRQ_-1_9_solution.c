
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    kernel_pid_t pid = thread_getpid();
    atomic_store(&counter, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting IRQ handler...\n");
    irq_set_handler(pid, irq_handler, NULL);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Waiting for interrupt...\n");
    while (atomic_load(&counter) == 0) {
        thread_yield();
    }

    printf("Interrupt received, counter: %d\n", atomic_load(&counter));

    return 0;
}
