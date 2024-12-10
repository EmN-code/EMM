
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/util.h>
#include <stdbool.h>
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
#include <time.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/init.h>
#include <zephyr/stats/stats.h>
#include <stdio.h>
#include <sys/time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/storage/flash_map.h>
#include <sys/types.h>
#include <limits.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/websocket.h>
#include <zephyr/types.h>

volatile bool interrupt_occurred = false;

void interrupt_handler(const void *arg)
{
    interrupt_occurred = true;
    printk("Interrupt occurred\n");
}

int main(void)
{
    // Register the interrupt handler
    irq_connect_dynamic(DT_IRQN(DT_NODELABEL(esp32_int)), 0, interrupt_handler, NULL, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(esp32_int)));

    printk("Waiting for interrupt...\n");

    while (1) {
        if (interrupt_occurred) {
            printk("Interrupt handled successfully\n");
            break;
        }
        k_sleep(K_MSEC(100));
    }

    return 0;
}
