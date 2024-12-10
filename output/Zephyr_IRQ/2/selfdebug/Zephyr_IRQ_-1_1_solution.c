#include <zephyr/types.h>
#include <zephyr/kernel.h>

/* Define atomic variable */
atomic_t my_atomic_var;

/* Interrupt service routine */
void my_isr(void *arg) {
    /* Increment atomic variable */
    atomic_inc(&my_atomic_var);
    
    /* Perform other ISR-related tasks */
}

/* Main function */
int main(void) {
    /* Initialize the atomic variable */
    atomic_set(&my_atomic_var, 0);

    /* Simulate interrupt setup - normally would configure hardware to trigger ISR */
    printk("Setting up interrupts and atomic variable\n");

    /* Simulate an interrupt call for demonstration purposes*/
    my_isr(NULL);

    /* Restore interrupts */
    printk("Atomic variable value after ISR: %d\n", atomic_get(&my_atomic_var));

    return 0;
}