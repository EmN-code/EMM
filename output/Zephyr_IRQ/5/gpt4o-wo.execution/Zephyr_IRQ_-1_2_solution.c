#include zephyr/toolchain.h
#include zephyr/sys/iterable_sections.h
#include zephyr/syscalls/random.h
#include zephyr/syscalls/i2c.h
#include zephyr/syscalls/kernel.h
#include zephyr/syscalls/flash.h
#include zephyr/syscalls/device.h
#include zephyr/sys/util.h
#include zephyr/sys/slist.h
#include zephyr/sys/mutex.h
#include zephyr/sys/math_extras.h
#include zephyr/sys/mem_stats.h
#include zephyr/sys/fdtable.h
#include zephyr/stats/stats.h
#include zephyr/storage/flash_map.h
#include zephyr/types.h
#include zephyr/tracing/tracing_macros.h
#include zephyr/rtio/rtio.h
#include zephyr/random/random.h
#include zephyr/posix/un.h
#include zephyr/posix/sys/socket.h
#include zephyr/posix/poll.h
#include zephyr/posix/netdb.h
#include zephyr/posix/arpa/inet.h
#include zephyr/pm/state.h
#include zephyr/net/wifi_mgmt.h
#include zephyr/net/wifi.h
#include zephyr/net/websocket.h
#include zephyr/net/tls_credentials.h
#include zephyr/net/socket_poll.h
#include zephyr/net/socket.h
#include zephyr/net/socket_select.h
#include zephyr/net/offloaded_netdev.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/net_ip.h
#include zephyr/net/net_if.h
#include zephyr/net/dns_resolve.h
#include zephyr/net/buf.h
#include zephyr/net/ethernet.h
#include zephyr/linker/sections.h
#include zephyr/kernel_includes.h
#include zephyr/kernel.h
#include zephyr/init.h
#include zephyr/devicetree.h
#include zephyr/device.h
#include zephyr/drivers/flash.h
#include zephyr/pm/device.h
#include sys/types.h

#define IRQ_PRIORITY 1

void my_isr_handler(const void *args) {
    printk("Interrupt occurred\n");
    // Handle interrupt
}

int main(void) {
    int irq_line = CONFIG_MY_DEVICE_IRQ;

    // Set up the ISR
    irq_connect_dynamic(irq_line, IRQ_PRIORITY, my_isr_handler, NULL, 0);

    // Enable the interrupt
    irq_enable(irq_line);

    while (1) {
        // Loop forever
        k_cpu_atomic_idle();
    }

    return 0;
}