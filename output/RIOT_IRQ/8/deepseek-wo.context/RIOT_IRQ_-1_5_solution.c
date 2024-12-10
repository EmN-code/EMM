
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t counter;

void irq_handler(void *arg)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    atomic_init(&counter, 0);

    printf("Initial counter value: %d\n", atomic_load(&counter));

    irq_set_handler(0, irq_handler, NULL);
    irq_trigger(0);

    printf("Counter value after interrupt: %d\n", atomic_load(&counter));

    return 0;
}
