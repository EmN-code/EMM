
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

static struct nvs_fs fs;

void init_nvs(void)
{
    int err;
    struct flash_area *fa;

    err = flash_area_open(DT_REG_ADDR(STORAGE_NODE), &fa);
    if (err) {
        LOG_ERR("Failed to open flash area (err %d)", err);
        return;
    }

    fs.flash_device = (const struct device *)fa->fa_dev;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 1;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to init NVS (err %d)", err);
    }

    flash_area_close(fa);
}

void read_sensor_and_store(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    struct sensor_value accel[3];
    int err;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample (err %d)", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor data (err %d)", err);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("Stored sensor data: %d, %d, %d", data[0], data[1], data[2]);
    }
}

int main(void)
{
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
