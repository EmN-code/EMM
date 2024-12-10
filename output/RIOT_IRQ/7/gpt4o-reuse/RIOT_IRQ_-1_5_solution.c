
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_NUM 11

static volatile int interrupt_occurred = 0;

void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = 1;
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable(IRQ_NUM);
    irq_set_handler(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Disable the interrupt
    irq_disable(IRQ_NUM);
    printf("Interrupt disabled\n");

    // Simulate some more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_NUM);
    printf("Interrupt re-enabled\n");

    // Simulate some more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Check if the interrupt occurred
    if (interrupt_occurred) {
        printf("Interrupt handling verified successfully\n");
    } else {
        printf("Interrupt did not occur as expected\n");
    }

    return 0;
}
