#include <xtimer.h>
#include <stdio.h>
#include <irq_arch.h>

volatile atomic_uint state;

// Dummy interrupt routine representing the interrupt service routine
void dummy_isr(void) {
    // Simulate some ISR work
    printf("ISR triggered\n");
    atomic_fetch_add(&state, 1);
}

int main(void) {
    state = 0;

    // Disable interrupts
    unsigned int prev_state = irq_disable();
    printf("Interrupts disabled, state: %u\n", prev_state);

    // Simulate some critical work by invoking dummy ISR
    dummy_isr();

    // Enable interrupts
    irq_restore(prev_state);
    printf("Interrupts restored, state: %u\n", state);

    // Ensure atomic variable reflects the ISR actions
    if (state == 1) {
        printf("ISR executed successfully and atomic state manipulated\n");
    } else {
        printf("ISR execution failed or state manipulation failed\n");
    }

    // Use xtimer_ticks64 to demonstrate time measurement
    xtimer_ticks64_t now = xtimer_ticks64();
    printf("Current time (ticks64): %llu\n", (unsigned long long)now.ticks64); 

    return 0;
}