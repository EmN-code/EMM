
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   "storage"
#define NVS_ID          1

struct nvs_fs nvs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = DT_REG_ADDR(DT_NODELABEL(storage)),
};

void read_sensor_data(void) {
    // Placeholder for reading MMA8x5x sensor data
    // This should be replaced with actual sensor reading code
    uint32_t sensor_data = 0x12345678; // Example data

    // Write sensor data to NVS
    ssize_t ret = nvs_write(&nvs, NVS_ID, &sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("NVS write success: %d bytes\n", (int)ret);
    }
}

int main(void) {
    // Initialize NVS
    int ret = nvs_init(&nvs, NVS_PARTITION);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    while (1) {
        read_sensor_data();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
