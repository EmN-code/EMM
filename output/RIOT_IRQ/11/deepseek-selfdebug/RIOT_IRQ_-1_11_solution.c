
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t flag = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);  // Set the atomic variable to 1
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Simulate some work
    xtimer_sleep(1);

    // Disable the IRQ
    irq_disable(IRQ_NUM);

    // Check the atomic variable
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
