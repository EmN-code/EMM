#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/arch/cpu.h>

int main() {
    // Initialize an atomic variable
    atomic_t my_atomic_var = ATOMIC_INIT(1);
    
    // Status of interrupts
    unsigned int key;

    // Print initial value of atomic variable
    printk("Initial atomic variable value: %d\n", atomic_get(&my_atomic_var));

    // Disable all interrupts and save the key
    key = irq_lock();
    printk("Interrupts disabled\n");

    // Work with the atomic variable
    atomic_clear(&my_atomic_var);
    printk("Atomic variable after clearing: %d\n", atomic_get(&my_atomic_var));

    // Restore the state of interrupts
    irq_unlock(key);
    printk("Interrupts restored\n");

    return 0;
}