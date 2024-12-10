#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>

// Atomic variable to verify interrupt enabling and disabling
atomic_t irq_disabled = ATOMIC_INIT(1);

// Interrupt handler function
void irq_handler(void *arg) {
    if (atomic_get(&irq_disabled)) {
        printk("Interrupts are disabled!\n");
    } else {
        printk("Handling interrupt!\n");
    }
}

// Thread function to enable and disable interrupts
void thread_func(void) {
    unsigned int key;

    // Disable interrupts
    key = irq_lock();
    atomic_set(&irq_disabled, 1);
    printk("Interrupts disabled\n");

    k_sleep(K_MSEC(2000)); // Simulate some processing

    // Enable interrupts
    irq_unlock(key);
    atomic_set(&irq_disabled, 0);
    printk("Interrupts enabled\n");
}

/* Main entry point of the program */
int main(void) {
    printk("Starting IRQ test program\n");

    // Setup and enable a dummy interrupt
    irq_connect_dynamic(DT_IRQN(DT_NODELABEL(i2c0)), 0, irq_handler, NULL, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(i2c0)));

    // Start a thread to control interrupt lock
    k_thread_create(&thread_data, stack_area, STACKSIZE,
                    (k_thread_entry_t)thread_func, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    return 0;
}

/* Configurations for the thread */
K_THREAD_STACK_DEFINE(stack_area, 1024);
static struct k_thread thread_data;
#define STACKSIZE 1024
#define PRIORITY 5