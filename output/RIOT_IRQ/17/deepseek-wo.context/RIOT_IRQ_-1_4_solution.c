
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    atomic_init(&counter, 0);

    irq_set_handler(0, irq_handler, NULL);
    irq_set(0, 1);

    printf("Counter before interrupt: %d\n", atomic_load(&counter));

    irq_set(0, 0);

    printf("Counter after interrupt: %d\n", atomic_load(&counter));

    return 0;
}
