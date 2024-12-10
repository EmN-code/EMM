
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter;

void irq_handler(void *arg)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    atomic_init(&counter, 0);

    irq_set_handler(0, irq_handler, NULL);
    irq_trigger(0);

    thread_sleep(1000000);

    int value = atomic_load(&counter);
    printf("Counter value: %d\n", value);

    return 0;
}
