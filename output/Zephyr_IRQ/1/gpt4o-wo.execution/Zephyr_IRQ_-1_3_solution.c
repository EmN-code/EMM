#include "time.h"
#include "zephyr/types.h"
#include "zephyr/net/socket_poll.h"
#include "zephyr/syscalls/device.h"
#include "zephyr/sys/util.h"
#include "zephyr/sys/mem_stats.h"
#include "zephyr/sys/iterable_sections.h"
#include "zephyr/syscalls/socket.h"
#include "zephyr/sys/printk.h"
#include "zephyr/net/dns_resolve.h"
#include "zephyr/net/mqtt.h"
#include "zephyr/net/coap.h"
#include "zephyr/net/socket.h"
#include "zephyr/net/mqtt_sn.h"
#include "zephyr/net/buf.h"
#include "zephyr/net/offloaded_netdev.h"
#include "zephyr/device.h"
#include "zephyr/devicetree.h"
#include "zephyr/linker/sections.h"
#include "zephyr/init.h"
#include "zephyr/kernel.h"
#include "zephyr/net/wifi.h"
#include "zephyr/net/websocket.h"
#include "zephyr/posix/netdb.h"
#include "zephyr/statistics/stats.h"
#include "zephyr/net/socket_select.h"
#include "zephyr/pm/state.h"
#include "zephyr/random/random.h"
#include "zephyr/storage/flash_map.h"
#include "zephyr/toolchain.h"
#include "zephyr/tracing/tracing_macros.h"
#include "zephyr/drivers/flash.h"


volatile int irq_disabled = 0;

void my_irq_handler(void *arg) {
    irq_disabled = 0;  // Indicate ISR was fired
}

int main() {
    irq_disabled = 1;  // Initially set to indicate IRQs are disabled

    /* Normally, you would set up your IRQs, etc. */
    // Example: 
    // int irq_line = CONFIG_TEST_IRQ_LINE;
    // IRQ_CONNECT(irq_line, CONFIG_TEST_IRQ_PRIO, my_irq_handler, NULL, 0);

    // Enable interrupts and check atomic section works
    // irq_enable(irq_line);

    // Check and verify that the interrupt handler works
    while (irq_disabled) {
        // spin wait until interrupt handles the business
    }

    return 0;
}
