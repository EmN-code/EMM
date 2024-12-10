
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/init.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/devicetree.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/toolchain.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <sys/types.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_STORAGE_NODE DT_NODELABEL(nvs_storage)

#define NVS_NUM_SECTORS DT_PROP(NVS_STORAGE_NODE, nvs_num_sectors)
#define NVS_SECTOR_SIZE DT_PROP(NVS_STORAGE_NODE, nvs_sector_size)
#define NVS_ADDRESS DT_REG_ADDR(NVS_STORAGE_NODE)
#define NVS_SIZE DT_REG_SIZE(NVS_STORAGE_NODE)

struct nvs_fs nvs_storage = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .offset = NVS_ADDRESS,
};

int main(void) {
    printk("Starting MMA8x5x sensor reading and NVS storage example\n");

    // Initialize NVS storage
    int ret = nvs_init(&nvs_storage, DT_REG_ADDR(NVS_STORAGE_NODE));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    // Main loop to read sensor data every 5 seconds
    while (1) {
        // Simulate reading from MMA8x5x sensor
        uint16_t sensor_value = sys_rand16_get();

        // Store the sensor value in NVS
        ret = nvs_write(&nvs_storage, 1, &sensor_value, sizeof(sensor_value));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("Sensor value %u stored in NVS\n", sensor_value);
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
