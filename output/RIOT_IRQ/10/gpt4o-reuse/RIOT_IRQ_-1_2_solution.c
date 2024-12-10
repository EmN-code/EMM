
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_NUM 11  // Assuming IRQ number 11 for ESP32

static volatile bool interrupt_occurred = false;

static void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the IRQ handler
    if (irq_is_valid(IRQ_NUM)) {
        irq_disable(IRQ_NUM);
        irq_set_handler(IRQ_NUM, irq_handler, NULL);
        irq_enable(IRQ_NUM);
    } else {
        printf("Invalid IRQ number\n");
        return 1;
    }

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Working... %d\n", i);

        // Check if interrupt occurred
        if (interrupt_occurred) {
            printf("Interrupt was handled correctly\n");
            interrupt_occurred = false;  // Reset the flag
        }
    }

    // Disable the IRQ
    irq_disable(IRQ_NUM);
    printf("Interrupt handling test completed\n");

    return 0;
}
