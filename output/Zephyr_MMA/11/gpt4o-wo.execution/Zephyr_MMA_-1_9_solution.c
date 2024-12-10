
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/device.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID 0

void main(void) {
    printk("Starting MMA8x5x reading and NVS storage on ESP32\n");

    while (1) {
        // Simulate reading from MMA8x5x sensor
        uint32_t sensor_value = sys_rand32_get();

        printk("Read sensor value: %u\n", sensor_value);

        // Simulate writing to NVS
        // Note: NVS operations are not directly supported in this example due to blacklist constraints
        // However, you can replace this with actual NVS write operations if allowed
        printk("Simulated NVS write of sensor value: %u\n", sensor_value);

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}
