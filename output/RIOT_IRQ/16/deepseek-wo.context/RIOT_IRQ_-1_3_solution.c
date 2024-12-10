
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_INIT(0);

void my_isr(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, my_isr, NULL);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Check the atomic variable
    int value = atomic_load(&counter);
    printf("Atomic counter value: %d\n", value);

    return 0;
}
