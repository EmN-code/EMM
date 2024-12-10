
#include <zephyr/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/toolchain.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/posix/poll.h>
#include <zephyr/devicetree.h>
#include <zephyr/random/random.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/init.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/websocket.h>
#include <zephyr/types.h>

volatile int atomic_var = 0;

void isr_handler(const void *arg)
{
    atomic_var = 1;
}

int main(void)
{
    // Register the interrupt handler
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(my_interrupt)), DT_IRQ(DT_NODELABEL(my_interrupt)), isr_handler, NULL, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt)));

    // Disable interrupts
    unsigned int key = irq_lock();

    // Perform some operations that should not be interrupted
    k_busy_wait(1000); // Simulate some work

    // Verify that the atomic variable is still 0
    if (atomic_var != 0) {
        printk("Error: Interrupt occurred while interrupts were disabled\n");
    } else {
        printk("Interrupts disabled successfully\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    k_busy_wait(1000000); // Wait for 1 second

    // Verify that the interrupt handler set the atomic variable
    if (atomic_var == 1) {
        printk("Interrupt handled successfully\n");
    } else {
        printk("Error: Interrupt was not handled\n");
    }

    return 0;
}
