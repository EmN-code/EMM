
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/slist.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/fdtable.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

struct nvs_fs nvs_storage = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .flash_device = FIXED_PARTITION_DEVICE(storage),
    .offset = FIXED_PARTITION_OFFSET(storage),
};

void read_sensor_data(void) {
    // Simulate reading data from MMA8x5x sensor
    uint32_t sensor_data = sys_rand32_get();

    // Store the reading value persistently using NVS
    ssize_t ret = nvs_write(&nvs_storage, 0, &sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("Sensor data stored: %u\n", sensor_data);
    }
}

int main(void) {
    // Initialize NVS
    int ret = nvs_mount(&nvs_storage);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return ret;
    }

    while (1) {
        read_sensor_data();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
