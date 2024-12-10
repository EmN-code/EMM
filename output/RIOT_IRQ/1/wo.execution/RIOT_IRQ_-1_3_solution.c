
#include <zephyr/kernel.h>
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
#include <zephyr/fs/nvs.h>
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
#include <stdio.h>
#include <sys/time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/storage/flash_map.h>
#include <limits.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/websocket.h>
#include <zephyr/types.h>

volatile bool interrupt_occurred = false;

void isr(void *arg) {
    ARG_UNUSED(arg);
    interrupt_occurred = true;
}

int main(void) {
    struct k_mutex mutex;
    k_mutex_init(&mutex);

    // Attach the interrupt service routine
    IRQ_CONNECT(CONFIG_ESP32_RTC_TIMER_IRQ, 0, isr, NULL, 0);
    irq_enable(CONFIG_ESP32_RTC_TIMER_IRQ);

    // Disable interrupts
    unsigned int key = irq_lock();

    // Perform some critical section code
    k_mutex_lock(&mutex, K_FOREVER);
    printk("Critical section: Mutex locked\n");
    k_sleep(K_MSEC(100));
    k_mutex_unlock(&mutex);
    printk("Critical section: Mutex unlocked\n");

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    while (!interrupt_occurred) {
        k_sleep(K_MSEC(10));
    }

    printk("Interrupt occurred!\n");

    return 0;
}
