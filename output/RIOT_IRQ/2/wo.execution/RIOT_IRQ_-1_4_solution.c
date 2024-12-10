
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>

#define MY_INTERRUPT_PRIORITY 1

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt handled!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Connect the interrupt service routine
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(my_interrupt_controller)), MY_INTERRUPT_PRIORITY, my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt_controller)));

    while (1) {
        if (atomic_get(&interrupt_flag)) {
            printk("Interrupt flag set, disabling interrupts...\n");
            irq_disable(DT_IRQN(DT_NODELABEL(my_interrupt_controller)));
            k_sleep(K_SECONDS(5));
            printk("Restoring interrupts...\n");
            irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt_controller)));
            atomic_set(&interrupt_flag, 0);
        }
        k_sleep(K_MSEC(100));
    }

    return 0;
}
