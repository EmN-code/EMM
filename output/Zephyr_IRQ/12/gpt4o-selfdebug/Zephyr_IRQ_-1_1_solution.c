#include zephyr/toolchain.h
#include zephyr/tracing/tracing_macros.h
#include zephyr/device.h
#include zephyr/kernel.h
#include zephyr/irq.h

// Declare an atomic variable
static atomic_t interrupt_counter;

// Interrupt Service Routine (ISR) for the interrupt
void test_irq_handler(const void *parameter)
{
    // Increment the atomic counter
    atomic_inc(&interrupt_counter);
}

int main()
{
    const int irq_line = 5; // Dummy IRQ line number for illustration
    
    // Initialize the atomic counter to 0
    atomic_set(&interrupt_counter, 0);

    // Connect the ISR to the IRQ line
    irq_connect(irq_line, 0, test_irq_handler, NULL, 0);

    // Enable the interrupt line
    irq_enable(irq_line);
    
    // Code to trigger the interrupt for testing
    
    // Simulating disabling and re-enabling interrupts globally
    unsigned int key = irq_lock();
    // Critical section: verify that interrupts are correctly disabled
    // Do some operations
    irq_unlock(key);

    // Check interrupt count and print result
    printk("Interrupt count: %d\n", atomic_get(&interrupt_counter));

    return 0;
}