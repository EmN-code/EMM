
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"
#include "zephyr/sys/printk.h"

#define STACKSIZE 1024
#define PRIORITY 7

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt occurred!\n");
}

void test_interrupt_handling(void)
{
    unsigned int key;

    printk("Disabling interrupts...\n");
    key = irq_lock();

    // Simulate some work
    k_busy_wait(100000);

    printk("Restoring interrupts...\n");
    irq_unlock(key);

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupts were restored correctly.\n");
    } else {
        printk("Interrupts were not restored correctly.\n");
    }
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32...\n");

    // Register the interrupt handler
    irq_connect_dynamic(DT_IRQN(DT_NODELABEL(esp32_intc)), 0, interrupt_handler, NULL, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(esp32_intc)));

    // Test the interrupt handling
    test_interrupt_handling();

    printk("Interrupt handling test completed.\n");
    return 0;
}
