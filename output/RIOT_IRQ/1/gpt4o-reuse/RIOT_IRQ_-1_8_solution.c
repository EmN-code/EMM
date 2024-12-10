
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second

static volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the interrupt
    irq_disable();
    irq_set(0, IRQ_PRIO_MEDIUM, irq_handler, NULL);
    irq_enable();

    // Test disabling and restoring interrupts
    printf("Disabling interrupts\n");
    unsigned state = irq_disable();
    xtimer_sleep(2);  // Sleep for 2 seconds with interrupts disabled
    printf("Restoring interrupts\n");
    irq_restore(state);

    // Wait for interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
