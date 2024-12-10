
#include <stdlib.h>
#include <esp_system.h>
#include <xtimer.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

// Define a simple atomic variable
volatile int atomic_var = 0;

// Function to handle interrupts
void irq_handler(void *arg)
{
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(ESP_INTR_NUM, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main loop iteration: %d\n", i);
    }

    // Verify the atomic variable
    if (atomic_var > 0) {
        printf("Interrupt handling verified successfully!\n");
    } else {
        printf("No interrupts were handled.\n");
    }

    return 0;
}
