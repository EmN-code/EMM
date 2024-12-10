#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>
#include <irq.h>
#include <atomic.h>

static atomic_t interrupt_count = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_inc(&interrupt_count);
    printf("Interrupt Count: %" PRIu32 "\n", interrupt_count);
}

int main(void)
{
    printf("RIOT Interrupt Handling Example\n");

    unsigned state;

    state = irq_disable();
    printf("Interrupts disabled\n");

    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Simulated work done while interrupts disabled.\n");

    printf("Re-enabling interrupts\n");
    irq_restore(state);

    printf("Interrupts re-enabled\n");

    while (1) {}

    return 0;
}