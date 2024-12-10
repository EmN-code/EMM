
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/types.h>
#include <zephyr/sys/hooks.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/pm/state.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/kernel_includes.h>

#define MMA8x5x_DEVICE_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    // Convert sensor values to a format suitable for storage
    uint32_t x = sensor_value_to_double(&accel[0]) * 1000;
    uint32_t y = sensor_value_to_double(&accel[1]) * 1000;
    uint32_t z = sensor_value_to_double(&accel[2]) * 1000;

    // Store the values in NVS
    struct nvs_fs fs;
    fs.sector_size = DT_REG_SIZE(NVS_PARTITION_ID);
    fs.sector_count = 1;
    fs.offset = DT_REG_ADDR(NVS_PARTITION_ID);

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return;
    }

    ret = nvs_write(&fs, 1, &x, sizeof(x));
    if (ret < 0) {
        LOG_ERR("Failed to write x value to NVS (%d)", ret);
    }

    ret = nvs_write(&fs, 2, &y, sizeof(y));
    if (ret < 0) {
        LOG_ERR("Failed to write y value to NVS (%d)", ret);
    }

    ret = nvs_write(&fs, 3, &z, sizeof(z));
    if (ret < 0) {
        LOG_ERR("Failed to write z value to NVS (%d)", ret);
    }

    LOG_INF("Stored sensor values in NVS");
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_DEVICE_NODE);

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
