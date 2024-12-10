#include <xtimer.h>  // Include for timer functionalities
#include <stdlib.h>   // Include for standard library 
#include <irq_arch.h> // Include for irq functionalities

static volatile unsigned int atomic_var = 0;  // Define a volatile atomic variable

void thread_func(void) {
    unsigned state;
    
    /* Disable interrupts and save the current state */
    state = irq_disable();
    
    /* Critical section: Modify the atomic variable */
    atomic_var++;

    /* Restore interrupts to previous state */
    irq_restore(state);
}

int main(void) {
    /* Simulate interrupt handling by calling the function */
    thread_func();
    
    /* Check if the atomic operation was successful */
    if (atomic_var == 1) {
        // Success: Print a message
        puts("Interrupt handling with atomic operation is successful.");
    } else {
        // Failure: Print a message
        puts("Interrupt handling with atomic operation failed.");
    }
    
    return 0;  
}
