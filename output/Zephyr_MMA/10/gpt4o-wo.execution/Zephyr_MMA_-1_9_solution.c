
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define SENSOR_NODE DT_NODELABEL(mma8x5x)

static struct nvs_fs fs;

void init_nvs(void)
{
    int err;
    const struct flash_area *fa;

    err = flash_area_open(NVS_PARTITION_ID, &fa);
    if (err) {
        LOG_ERR("Failed to open flash area %d", NVS_PARTITION_ID);
        return;
    }

    fs.flash_device = (uintptr_t)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = 4096; // Adjust based on your flash sector size
    fs.sector_count = 2;   // Adjust based on your flash size

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to initialize NVS (%d)", err);
        return;
    }

    flash_area_close(fa);
}

void read_sensor_and_store(void)
{
    const struct device *dev = DEVICE_DT_GET(SENSOR_NODE);
    struct sensor_value accel[3];
    int err;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device %s is not ready", dev->name);
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample from %s", dev->name);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor data from %s", dev->name);
        return;
    }

    // Store the sensor data in NVS
    err = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (%d)", err);
    } else {
        LOG_INF("Stored sensor data in NVS");
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
