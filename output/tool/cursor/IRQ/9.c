#include <stdio.h>
#include <xtensa/hal.h>  // For ESP32 specific functions
#include <atomic.h>
#include <thread.h>

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_interrupt_handler(void)
{
    // Set the atomic flag to indicate the interrupt was handled
    atomic_set(&interrupt_flag, 1);
}

void disable_interrupts(void)
{
    // Disable interrupts
    xtensa_int_disable();
}

void enable_interrupts(void)
{
    // Enable interrupts
    xtensa_int_enable();
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initially disable interrupts
    disable_interrupts();

    // Simulate an interrupt
    my_interrupt_handler();

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag)) {
        printf("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printf("Interrupt was not handled while interrupts were disabled.\n");
    }

    // Enable interrupts
    enable_interrupts();

    // Reset the flag
    atomic_set(&interrupt_flag, 0);

    // Simulate another interrupt
    my_interrupt_handler();

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag)) {
        printf("Interrupt was handled after enabling interrupts.\n");
    } else {
        printf("Interrupt was not handled after enabling interrupts.\n");
    }

    return 0;
}