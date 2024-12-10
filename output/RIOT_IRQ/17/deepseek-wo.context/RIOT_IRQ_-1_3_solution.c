
#include <stdio.h>
#include <stdint.h>
#include "xtimer.h"
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"

#define TEST_VALUE (42)

static atomic_int_t atomic_var;

void my_irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, TEST_VALUE);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_init(&atomic_var, 0);

    // Set the interrupt handler
    irq_set_handler(0, my_irq_handler, NULL);

    // Enable the interrupt
    irq_set(0, 1);

    // Wait for the interrupt to be triggered
    xtimer_sleep(1);

    // Check the value of the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == TEST_VALUE) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed! Expected %d, got %d\n", TEST_VALUE, value);
    }

    return 0;
}
