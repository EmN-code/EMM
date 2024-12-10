#include "zephyr/toolchain.h"
#include "zephyr/sys/util.h"
#include "zephyr/sys/mutex.h"
#include "zephyr/sys/fdtable.h"
#include "zephyr/syscalls/device.h"
#include "zephyr/device.h"
#include "zephyr/kernel.h"

void my_interrupt_handler(void *arg) {
    int key = irq_lock();

    printk("Handling interrupt\n");

    irq_unlock(key);
}

int main(void) {
    printk("Starting ESP32 interrupt handling example\n");

    /* Initialize interrupt */
    irq_disable(0);
    irq_connect_dynamic(0, 0, my_interrupt_handler, NULL, 0);
    irq_enable(0);

    /* Simulate an interrupt call */
    my_interrupt_handler(NULL);

    /* Spin forever */
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}