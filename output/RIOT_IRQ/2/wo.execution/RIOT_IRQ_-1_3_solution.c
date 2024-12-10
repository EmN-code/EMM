
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/offloaded_netdev.h>
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
#include <zephyr/net/socket.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/init.h>
#include <zephyr/stats/stats.h>
#include <sys/time.h>
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

#define MY_INTERRUPT_CONTROLLER DT_NODELABEL(my_interrupt_controller)
#define MY_INTERRUPT_IRQ DT_IRQN(MY_INTERRUPT_CONTROLLER)

volatile bool interrupt_occurred = false;

void my_interrupt_handler(const void *arg)
{
    interrupt_occurred = true;
    printk("Interrupt occurred!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Configure the interrupt
    IRQ_CONNECT(MY_INTERRUPT_IRQ, 0, my_interrupt_handler, NULL, 0);
    irq_enable(MY_INTERRUPT_IRQ);

    // Main loop to check for interrupt occurrence
    while (1) {
        if (interrupt_occurred) {
            printk("Interrupt handled correctly.\n");
            interrupt_occurred = false;
        }
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
