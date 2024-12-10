
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    unsigned state = irq_disable();

    irq_set_handler(0, irq_handler, NULL);

    irq_restore(state);

    irq_trigger(0);

    while (atomic_load(&flag) == 0) {
        thread_yield();
    }

    printf("Interrupt handling verified successfully.\n");

    return 0;
}
