
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

    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    while (1) {
        if (atomic_load(&counter) == 1) {
            printf("Interrupt handled successfully!\n");
            atomic_store(&counter, 0);
        }
        thread_yield();
    }

    return 0;
}
