
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"
#include "xtimer.h"

#define TEST_VALUE (42)

static atomic_int_t atomic_var;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, TEST_VALUE);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    xtimer_sleep(1);

    int value = atomic_load(&atomic_var);
    if (value == TEST_VALUE) {
        printf("Interrupt handling and atomic operations work correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
