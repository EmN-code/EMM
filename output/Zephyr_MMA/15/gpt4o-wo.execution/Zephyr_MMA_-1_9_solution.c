
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_SECTOR_COUNT DT_PROP(DT_NODELABEL(storage), nvs_num_sectors)
#define NVS_SECTOR_SIZE DT_PROP(DT_NODELABEL(storage), size)

static struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = FIXED_PARTITION_OFFSET(storage),
};

void read_mma8x5x(const struct device *dev, int *value)
{
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    *value = accel[0].val1;
}

void store_nvs(int value)
{
    if (nvs_write(&nvs, 1, &value, sizeof(value)) < 0) {
        LOG_ERR("Failed to write to NVS");
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

int main(void)
{
    const struct device *dev = device_get_binding("MMA8X5X");
    int value;

    if (!dev) {
        LOG_ERR("Device not found");
        return -1;
    }

    if (nvs_mount(&nvs) < 0) {
        LOG_ERR("Failed to mount NVS");
        return -1;
    }

    while (1) {
        read_mma8x5x(dev, &value);
        store_nvs(value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
