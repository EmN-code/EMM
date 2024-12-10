#include <zephyr/toolchain.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/irq.h>

/* Define Constants */
#define STACKSIZE 1024
#define PRIORITY 7

/* Thread stack and data */
K_THREAD_STACK_DEFINE(thread_stack, STACKSIZE);
struct k_thread thread_data;

/* Atomic variable for interrupt verification */
atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr_handler(void *arg) {
    ARG_UNUSED(arg);
    /* Simulate interrupt handling */
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt handled, flag set to 1.\n");
}

void thread_entry(void) {
    while (1) {
        /* Do nothing, just sleep */
        k_msleep(1000);
    }
}

int main() {
    /* Creating a thread to demonstrate multi-threading along with interrupt handling */
    k_thread_create(&thread_data, thread_stack, STACKSIZE,
                    (k_thread_entry_t)thread_entry, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
    
    /* Set up an interrupt */
    int irq_line = 11;  // Suppose an arbitrary line for the purpose of demonstration
    irq_connect_dynamic(irq_line, PRIORITY, my_isr_handler, NULL, 0);
    
    /* Enable this interrupt line */
    irq_enable(irq_line);
    
    while (1) {
        if (atomic_get(&interrupt_flag)) {
            printk("Interrupt verified, flag is 1.\n");
            /* Reset the flag */
            atomic_clear(&interrupt_flag);
        }
        k_msleep(500);
    }
    return 0;
}