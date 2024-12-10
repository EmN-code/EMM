
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/interrupt_controller/intc_esp32.h>

#define MY_INTERRUPT_CONTROLLER DT_NODELABEL(my_interrupt_controller)
#define MY_IRQ_IDX 0

static volatile int interrupt_flag = 0;

void my_isr(const void *arg)
{
    interrupt_flag = 1;
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MY_INTERRUPT_CONTROLLER);
    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return 0;
    }

    int irq = DT_IRQN(MY_INTERRUPT_CONTROLLER, MY_IRQ_IDX);
    int ret = irq_connect_dynamic(irq, 0, my_isr, NULL, 0);
    if (ret < 0) {
        printk("Failed to connect IRQ\n");
        return 0;
    }

    irq_enable(irq);

    while (1) {
        if (interrupt_flag) {
            printk("Interrupt occurred\n");
            interrupt_flag = 0;
        }
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
