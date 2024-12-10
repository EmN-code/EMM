#include <zephyr/toolchain.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/arch/cpu.h>

static atomic_t interrupt_counter;

void my_isr(const void *arg) {
    atomic_inc(&interrupt_counter);
}

void main(void) {
    atomic_set(&interrupt_counter, 0);

    unsigned int key = irq_lock();
    // Simulate critical section
    __asm__ volatile ("nop");
    irq_unlock(key);

    // Setup and trigger interrupt
    irq_connect_dynamic(DT_IRQN(DT_INST(0, vnd_device)), IRQ_DEFAULT_PRIORITY, my_isr, NULL, 0);
    irq_enable(DT_IRQN(DT_INST(0, vnd_device)));

    // Optional: cause interrupt for testing
    NVIC_SetPendingIRQ(DT_IRQN(DT_INST(0, vnd_device)));

    // Verify the interrupt counter has incremented
    int count = atomic_get(&interrupt_counter);
    printk("Interrupt counter: %d\n", count);
}
