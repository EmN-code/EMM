
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
#include <zephyr/kernel_includes.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/coap.h>
#include <zephyr/device.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <zephyr/devicetree.h>
#include <zephyr/posix/poll.h>
#include <zephyr/random/random.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/slist.h>
#include <zephyr/init.h>
#include <zephyr/stats/stats.h>
#include <zephyr/sys/printk.h>
#include <sys/time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/websocket.h>
#include <zephyr/types.h>

volatile int interrupt_flag = 0;

void isr_handler(void *arg)
{
    ARG_UNUSED(arg);
    interrupt_flag = 1;
}

int main(void)
{
    // Initialize the interrupt flag
    interrupt_flag = 0;

    // Configure the interrupt
    irq_connect_dynamic(DT_IRQN(DT_NODELABEL(my_interrupt)), 0, isr_handler, NULL, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt)));

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printk("Working... %d\n", i);
        k_sleep(K_SECONDS(1));

        // Check if the interrupt was triggered
        if (interrupt_flag) {
            printk("Interrupt was triggered!\n");
            interrupt_flag = 0; // Reset the flag
        }
    }

    return 0;
}
