
#include "irq.h"
#include "atomic_utils.h"
#include "thread.h"
#include "xtimer.h"

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    xtimer_sleep(1);

    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
