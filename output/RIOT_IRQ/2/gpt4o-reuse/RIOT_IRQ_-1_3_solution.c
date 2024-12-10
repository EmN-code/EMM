
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define ATOMIC_VAR_INIT(val) (val)
#define atomic_read(v) (*(volatile typeof(*v) *)(v))
#define atomic_set(v, i) (*(volatile typeof(*v) *)(v) = (i))

static volatile int atomic_var = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_set(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set(0, interrupt_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Check if the interrupt was handled
    if (atomic_read(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
