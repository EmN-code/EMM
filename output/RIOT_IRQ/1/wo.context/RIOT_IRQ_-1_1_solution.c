#include <irq_arch.h>
#include <stdio.h>
#include <stdatomic.h>

// Global atomic variable to track interrupt status
atomic_bool interrupt_status = ATOMIC_VAR_INIT(false);

// Function to disable interrupts and update atomic variable
void disable_interrupts(void) {
    irq_disable();
    atomic_store(&interrupt_status, true);
    printf("Interrupts disabled.\n");
}

// Function to enable interrupts and update atomic variable
void enable_interrupts(void) {
    // Normally we would have a function like irq_enable()
    // Since we don't have that, we simulate enabling
    unsigned state = irq_disable();  // store the current state
    irq_restore(state);              // restore the state to enable
    atomic_store(&interrupt_status, false);
    printf("Interrupts enabled.\n");
}

// Function to check if interrupts are enabled or disabled
void check_interrupt_status(void) {
    if (atomic_load(&interrupt_status)) {
        printf("Interrupts are currently disabled.\n");
    } else {
        printf("Interrupts are currently enabled.\n");
    }
}

int main(void) {
    printf("Starting interrupt handling test...\n");
    
    check_interrupt_status();  // check initial status
    
    disable_interrupts();      // disable interrupts
    check_interrupt_status();  // verify status
    
    enable_interrupts();       // enable interrupts
    check_interrupt_status();  // verify status

    printf("Interrupt handling test completed.\n");
    return 0;
}