
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_I2C_BUS DT_LABEL(DT_NODELABEL(i2c0))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    err = flash_get_sectors(fs.flash_device, fs.sector_count, sectors);
    if (err) {
        LOG_ERR("Failed to get flash sectors: %d", err);
        return;
    }

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to init NVS: %d", err);
        return;
    }
}

void store_reading(int16_t reading) {
    int err;
    err = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Stored reading: %d", reading);
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int16_t reading;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    reading = (int16_t)(accel[0].val1); // Simplified for example
    store_reading(reading);
}

int main(void) {
    const struct device *i2c_dev = device_get_binding(MMA8x5x_I2C_BUS);
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("MMA8x5x device not found");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
