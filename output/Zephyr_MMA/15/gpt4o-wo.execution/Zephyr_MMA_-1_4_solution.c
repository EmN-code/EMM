
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/poll.h>
#include <time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define SENSOR_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION_ID DT_NODELABEL(storage)

struct nvs_fs nvs;

void read_mma8x5x(struct sensor_value *accel_values) {
    const struct device *dev = DEVICE_DT_GET(SENSOR_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    if (sensor_sample_fetch(dev) < 0) {
        printk("Failed to fetch sensor sample\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel_values) < 0) {
        printk("Failed to get sensor channel\n");
        return;
    }
}

void store_nvs(struct sensor_value *accel_values) {
    uint8_t data[sizeof(struct sensor_value) * 3];
    memcpy(data, accel_values, sizeof(struct sensor_value) * 3);

    if (nvs_write(&nvs, 1, data, sizeof(data)) < 0) {
        printk("Failed to write to NVS\n");
    } else {
        printk("Data stored in NVS\n");
    }
}

int main(void) {
    struct sensor_value accel_values[3];

    // Initialize NVS
    nvs.flash_device = (const struct device *)FIXED_PARTITION_DEVICE(NVS_PARTITION_ID);
    nvs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    nvs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION_ID);
    nvs.sector_count = 1;

    if (nvs_mount(&nvs) < 0) {
        printk("Failed to mount NVS\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(accel_values);
        store_nvs(accel_values);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
