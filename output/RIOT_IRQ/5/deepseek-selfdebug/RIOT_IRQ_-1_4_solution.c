
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int_t counter;

void isr_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    atomic_init(&counter, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(0, isr_handler, NULL);

    printf("Enabling interrupts...\n");
    irq_restore(state);

    printf("Waiting for interrupt...\n");
    while (atomic_load(&counter) == 0) {
        thread_yield();
    }

    printf("Interrupt received, counter value: %d\n", atomic_load(&counter));

    return 0;
}
