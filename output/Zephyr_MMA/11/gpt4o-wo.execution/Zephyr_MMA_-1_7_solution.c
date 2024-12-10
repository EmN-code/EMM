
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage_partition)

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    ret = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Stored sensor data: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    struct flash_area fa;
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    ret = flash_area_open(DT_FLASH_AREA_ID(STORAGE_PARTITION), &fa);
    if (ret) {
        LOG_ERR("Failed to open flash area: %d", ret);
        return -EIO;
    }

    fs.flash_device = fa.fa_device;
    fs.offset = fa.fa_off;
    fs.sector_size = DT_PROP(STORAGE_PARTITION, erase_block_size);
    fs.sector_count = DT_PROP(STORAGE_PARTITION, size) / fs.sector_size;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("Failed to initialize NVS: %d", ret);
        flash_area_close(&fa);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    flash_area_close(&fa);
    return 0;
}
