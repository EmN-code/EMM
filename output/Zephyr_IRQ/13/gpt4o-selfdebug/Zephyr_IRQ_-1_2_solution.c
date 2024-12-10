#include <zephyr/zephyr.h>
#include <zephyr/sys/atomic.h>

atomic_t interrupt_counter;

void my_interrupt_handler(void)
{
    atomic_inc(&interrupt_counter);
}

int main(void)
{
    // Initially, clear the interrupt counter
    atomic_clear(&interrupt_counter);
    
    // Simulate enabling interrupts and attach the handler
    irq_enable(my_interrupt_handler);

    while (1) {
        // Here, if the interrupt handler is executed, the count will increase

        if (atomic_get(&interrupt_counter) > 0) {
            // If interrupt occurred, reset the counter
            printk("Interrupt occurred. Counter reset.\n");
            atomic_clear(&interrupt_counter);
        }

        k_sleep(K_MSEC(500)); // Simulate doing other work
    }

    return 0;
}