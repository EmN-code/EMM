
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

static struct nvs_fs fs;

void init_nvs(void)
{
    struct flash_area *fa;
    int err;

    err = flash_area_open(DT_FLASH_AREA_ID(NVS_PARTITION), &fa);
    if (err) {
        LOG_ERR("Failed to open flash area: %d", err);
        return;
    }

    fs.flash_device = (const struct device *)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 3;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to initialize NVS: %d", err);
        return;
    }

    flash_area_close(fa);
}

void read_sensor_and_store(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    struct sensor_value accel[3];
    int err;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor channel: %d", err);
        return;
    }

    err = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Sensor data stored successfully");
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
