
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
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return;
    }

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

    uint8_t data[3];
    data[0] = (uint8_t)sensor_value_to_double(&accel[0]);
    data[1] = (uint8_t)sensor_value_to_double(&accel[1]);
    data[2] = (uint8_t)sensor_value_to_double(&accel[2]);

    ret = nvs_write(fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_DEVICE_NODE);
    struct nvs_fs fs;
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -ENODEV;
    }

    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_SECTOR_SIZE(NVS_PARTITION_ID);
    fs.sector_count = FIXED_PARTITION_SIZE(NVS_PARTITION_ID) / fs.sector_size;

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
