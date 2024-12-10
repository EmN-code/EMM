// C Code for Zephyr (ESP32) Interrupt Handling with Atomic Variables

#include <zephyr/sys/util.h>
#include <zephyr/types.h>
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <kernel.h>
#include <sys/fdtable.h>
#include <net/socket_select.h>
#include <net/socket_poll.h>
#include <net/socket.h>
#include <net/websocket.h>
#include <net/wifi.h>
#include <random/random.h>
#include <tracing/tracing_macros.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/sys/socket.h>
#include <sys/mem_stats.h>
#include <sys/mutex.h>
#include <sys/time.h>
#include <stats/stats.h>
#include <kernel_includes.h>
#include <toolchain.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/init.h>
#include <linker/sections.h>
#include <devicetree.h>

#define MY_IRQ_LINE 5

volatile uint32_t my_atomic_counter = 0;

void simulate_isr(void) {
    // This function is supposed to simulate an ISR
    my_atomic_counter++;
}

void my_irq_setup_handler(void) {
    if (MY_IRQ_LINE) {
        simulate_isr();
    }
}

void main(void) {
    printk("Starting the interrupt simulation with atomic variable\n");
    // Simulate enabling interrupts which runs ISR
    my_irq_setup_handler();
    printk("Atomic counter after ISR: %d\n", my_atomic_counter);
}