
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_LABEL "storage"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_area *fap;

    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    err = flash_area_open(FIXED_PARTITION_ID(NVS_PARTITION_LABEL), &fap);
    if (err) {
        LOG_ERR("Failed to open flash area (err %d)", err);
        return;
    }

    err = nvs_init(&fs, fap->fa_device);
    if (err) {
        LOG_ERR("Failed to initialize NVS (err %d)", err);
        flash_area_close(fap);
        return;
    }

    flash_area_close(fap);
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor sample update failed");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    err = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
