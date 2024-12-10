
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    kernel_pid_t pid;

    puts("RIOT interrupt handling example");

    // Set the interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Disable interrupts and check if the counter is still 0
    irq_disable();
    if (atomic_load(&counter) == 0) {
        puts("Interrupts disabled correctly");
    } else {
        puts("Interrupts were not disabled correctly");
    }

    // Restore interrupts and check if the counter is 1
    irq_restore(irq_enable());
    if (atomic_load(&counter) == 1) {
        puts("Interrupts restored correctly");
    } else {
        puts("Interrupts were not restored correctly");
    }

    return 0;
}
